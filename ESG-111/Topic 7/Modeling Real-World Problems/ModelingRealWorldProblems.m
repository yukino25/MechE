function [xl, xd, ya, ys] = carbonxchnge(xl0, xd0, ya0, ys0, N, dt, params)

    % parameters
    k1 = params(1);
    k2 = params(2);
    k3 = params(3);
    k4 = params(4);
    k5 = params(5);

    % Total carbon
    a = xl0 + xd0 + ya0 + ys0;

    % output arrays
    xl = zeros(1, N+1);
    xd = zeros(1, N+1);
    ya = zeros(1, N+1);
    ys = zeros(1, N+1);

    % initial conditions
    xl(1) = xl0;
    xd(1) = xd0;
    ya(1) = ya0;
    ys(1) = ys0;

    for n = 1:N
        dxl = k1 * xl(n) * ya(n) - k2 * xl(n);
        dxd = k2 * xl(n) - k3 * xd(n);
        dys = k4 * (ya(n) - k5 * ys(n));

        xl(n+1) = xl(n) + dt * dxl;
        xd(n+1) = xd(n) + dt * dxd;
        ys(n+1) = ys(n) + dt * dys;

        ya(n+1) = a - xl(n+1) - xd(n+1) - ys(n+1);
    end
end




% Parameters
k1 = 4.3e-5;   % cm^2 mg^-1 yr^-1
k2 = 2.0e-2;   % yr^-1
k3 = 1.6e-2;   % yr^-1
k5 = 1.8e-2;   % yr^-1

% k4 is arbitrary
k4 = k5;

params = [k1, k2, k3, k4, k5];

% Initial conditions
xl0 = 400;      % mg cm^-2
xd0 = 500;      % mg cm^-2
ya0 = 460;      % mg cm^-2
ys0 = 25000;    % mg cm^-2
T  = 600;       % Total simulation time [years]
fs = 100;       % Samples per year
N  = T * fs;    % Number of time steps
dt = 1 / fs;    % Time step [years]
t  = (0:N) * dt;

% basic model, no time lag
[xl, xd, ya, ys] = carbonxchnge(xl0, xd0, ya0, ys0, N, dt, params);

figure('Name', 'Carbon Cycle - Basic Model', 'NumberTitle', 'off');
subplot(2,2,1);
plot(t, xl, 'g', 'LineWidth', 1.5);
xlabel('Time (years)'); ylabel('Carbon (mg cm^{-2})');
title('Living Plants (x_l)'); grid on;

subplot(2,2,2);
plot(t, xd, 'r', 'LineWidth', 1.5);
xlabel('Time (years)'); ylabel('Carbon (mg cm^{-2})');
title('Dead Organic Matter (x_d)'); grid on;

subplot(2,2,3);
plot(t, ya, 'b', 'LineWidth', 1.5);
xlabel('Time (years)'); ylabel('Carbon (mg cm^{-2})');
title('Atmosphere (y_a)'); grid on;

subplot(2,2,4);
plot(t, ys, 'c', 'LineWidth', 1.5);
xlabel('Time (years)'); ylabel('Carbon (mg cm^{-2})');
title('Sea (y_s)'); grid on;

sgtitle('Carbon Cycle Model (Eriksson & Welander, 1956)');


% Time lag
tau_values = [10, 50, 100];   % Years
colors = {'g', 'm', 'b'};
legend_entries = {};

figure('Name', 'Carbon Cycle - Time-Lag Model', 'NumberTitle', 'off');

for idx = 1:length(tau_values)
    tau = tau_values(idx);

    % output arrays
    xl_lag = zeros(1, N+1);
    xd_lag = zeros(1, N+1);
    ya_lag = zeros(1, N+1);
    ys_lag = zeros(1, N+1);
    s_lag  = zeros(1, N+1);

    % Initial conditions, s starts equal to xl
    xl_lag(1) = xl0;
    xd_lag(1) = xd0;
    ya_lag(1) = ya0;
    ys_lag(1) = ys0;
    s_lag(1)  = xl0;

    a = xl0 + xd0 + ya0 + ys0;

    for n = 1:N
        dxl = k1 * xl_lag(n) * ya_lag(n) - k2 * s_lag(n); 
        dxd = k2 * s_lag(n) - k3 * xd_lag(n);
        dys = k4 * (ya_lag(n) - k5 * ys_lag(n));
        ds  = (xl_lag(n) - s_lag(n)) / tau;

        xl_lag(n+1) = xl_lag(n) + dt * dxl;
        xd_lag(n+1) = xd_lag(n) + dt * dxd;
        ys_lag(n+1) = ys_lag(n) + dt * dys;
        s_lag(n+1)  = s_lag(n)  + dt * ds;

        ya_lag(n+1) = a - xl_lag(n+1) - xd_lag(n+1) - ys_lag(n+1);
    end

    subplot(2,2,1); hold on;
    plot(t, xl_lag, colors{idx}, 'LineWidth', 1.5);

    subplot(2,2,2); hold on;
    plot(t, xd_lag, colors{idx}, 'LineWidth', 1.5);

    subplot(2,2,3); hold on;
    plot(t, ya_lag, colors{idx}, 'LineWidth', 1.5);

    subplot(2,2,4); hold on;
    plot(t, ys_lag, colors{idx}, 'LineWidth', 1.5);

    legend_entries{end+1} = sprintf('\\tau = %d yr', tau);
end

subplot(2,2,1); xlabel('Time (years)'); ylabel('Carbon (mg cm^{-2})');
title('Living Plants (x_l)'); grid on; legend(legend_entries);

subplot(2,2,2); xlabel('Time (years)'); ylabel('Carbon (mg cm^{-2})');
title('Dead Organic Matter (x_d)'); grid on; legend(legend_entries);

subplot(2,2,3); xlabel('Time (years)'); ylabel('Carbon (mg cm^{-2})');
title('Atmosphere (y_a)'); grid on; legend(legend_entries);

subplot(2,2,4); xlabel('Time (years)'); ylabel('Carbon (mg cm^{-2})');
title('Sea (y_s)'); grid on; legend(legend_entries);

sgtitle('Carbon Cycle Model with Time Lag (\tau)');