%code simplified mathematical model of the carbon cycle.
%Carbon is exchanged and transformed in various forms through various natural processes.
%Creating a computational model helps us investigate trends and the influences of various factors

% Create a function to estimate the amount of carbon in each "bucket" over time,
% given initial conditions and the following parameter values.
% Your function should accept a vector with the parameter values as an input.
% The function prototype should be
%  [X1,Xd,Ya,Ys] = carbonxchange(X1_0,Xd_0,Ya_0,Ys_0,N,dt,params)

% Run the model for 600 years with 100 samples per year. Plot the values over time.
% rate of terrestric net assimilation of perineal plants 8 mg cm^-2 yr^-1
% k4 value is abitrary
% The first step in Eulers method is to convert our differential equations
% into finite difference equations. To do this, we need to consider the definition of the first derivative:
% dx_dt = (x_n+1 - x_n) / dt

% Define carbon states
X1_0 = 400; %mg Cm^-2
Xd_0 = 500; %mg Cm^-2
Ya_0 = 460; %mg Cm^-2
Ys_0 = 25000; %mg Cm^-2
k1 = 4.3e-5; % cm^2 mg^-1 yr^-1
k2 = 2.0e-2; % yr^-1
k3 = 1.6e-2; % yr^-1
k4 = 1; % yr^-1
k5 = 1.8e-2; % yr^-1
params = [k1, k2, k3, k4, k5];
N = 600 * 100; % total number of time steps (600 years with 100 samples per year)
dt = 1 / 100; % time step in years

[X1, Xd, Ya, Ys] = carbonxchange(X1_0, Xd_0, Ya_0, Ys_0, N, dt, params);

% carbon cycle function
function [X1, Xd, Ya, Ys] = carbonxchange(X1_0, Xd_0, Ya_0, Ys_0, N, dt, params)
    % Unpack parameters
    k1 = params(1);
    k2 = params(2);
    k3 = params(3);
    k4 = params(4);
    k5 = params(5);

    % Initialize arrays to store results
    X1 = zeros(N, 1);
    Xd = zeros(N, 1);
    Ya = zeros(N, 1);
    Ys = zeros(N, 1);

    % Set initial conditions
    X1(1) = X1_0;
    Xd(1) = Xd_0;
    Ya(1) = Ya_0;
    Ys(1) = Ys_0;

    % Time loop (Euler's method)
    for t = 2:N
        dX1_dt = k1 * X1(t-1) * Ya(t-1) - k2 * X1(t-1); % photosynthesis minus plant respiration
        dXd_dt = k2 * X1(t-1) - k3 * Xd(t-1);            % plant death minus decomposition
        dYs_dt = k4 * (Ya(t-1) - k5 * Ys(t-1));           % ocean absorption and release
        dYa_dt = -dX1_dt - dXd_dt - dYs_dt;               % atmosphere: conservation of total carbon

        % Update states using Euler's method
        X1(t) = X1(t-1) + dX1_dt * dt;
        Xd(t) = Xd(t-1) + dXd_dt * dt;
        Ys(t) = Ys(t-1) + dYs_dt * dt;
        Ya(t) = Ya(t-1) + dYa_dt * dt;
    end

    % Plot results
    time = (0:N-1)' * dt;

    figure;
    plot(time, X1, 'r');
    legend('X1 (Terrestrial Carbon)');
    xlabel('Time (years)');
    ylabel('Carbon Amount (mg Cm^{-2})');
    title('Terrestrial Carbon (X1) over Time');

    figure;
    plot(time, Xd, 'g');
    legend('Xd (Decomposed Carbon)');
    xlabel('Time (years)');
    ylabel('Carbon Amount (mg Cm^{-2})');
    title('Decomposed Carbon (Xd) over Time');

    figure;
    plot(time, Ya, 'b');
    legend('Ya (Atmospheric Carbon)');
    xlabel('Time (years)');
    ylabel('Carbon Amount (mg Cm^{-2})');
    title('Atmospheric Carbon (Ya) over Time');

    figure;
    plot(time, Ys, 'c');
    legend('Ys (Ocean Carbon)');
    xlabel('Time (years)');
    ylabel('Carbon Amount (mg Cm^{-2})');
    title('Ocean Carbon (Ys) over Time');
end
