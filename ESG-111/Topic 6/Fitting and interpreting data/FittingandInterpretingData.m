%Graph the stress-strain diagram for the dataset. 
%Determine the linear region of the curve; find the linear regression and R2 value for that line. 
%The slope of the linear regression will be the materials stiffness, also called the modulus of elasticity (E).  
%Determine the ultimate stress and failure stress of the material.  
%Compare the determined value of ultimate stress and modulus of elasticity to those from matweb.com for the material; 
%Calculate and report the percent error for both properties.

%Output to the command window the following calculated properties of the steel in the units specified:  
%Modulus of Elasticity [GPa]
%R2 value for the Modulus of Elasticity 
%Ultimate stress [MPa]
%Failure stress [MPa]





Table = readtable('ESG-111_TensileTest Data.csv');
AxialStrain = Table{:, 8};
Load_N = Table{:, 7};
Extension_mm = Table{:, 5}; 
Time_sec = Table{:, 4};
Intitial_Width_mm = 4;
Initial_Thickness_mm = 0.25;

% Calculate cross-sectional area and stress
Area_mm2 = Intitial_Width_mm * Initial_Thickness_mm;
Stress_MPa = Load_N / Area_mm2;

% Plot stress vs axial strain
figure;
plot(AxialStrain, Stress_MPa, 'b-', 'LineWidth', 1.5);
xlabel('Axial Strain [--]');
ylabel('Stress [MPa]');
title('Stress-Strain Diagram');
grid on



%linear region
threshold = 0.999;
linear_start = 2;
linear_end = 3;

for i = 4:length(AxialStrain)
    x = AxialStrain(linear_start:i);
    y = Stress_MPa(linear_start:i);
    p = polyfit(x, y, 1);
    y_fit = polyval(p, x);
    SS_res = sum((y - y_fit).^2);
    SS_tot = sum((y - mean(y)).^2);
    R2 = 1 - SS_res/SS_tot;

    if R2 < threshold
        linear_end = i - 1;
        break;
    end
end

strain_linear = AxialStrain(linear_start:linear_end);
stress_linear = Stress_MPa(linear_start:linear_end);

%modulus of elasticity
p = polyfit(strain_linear, stress_linear, 1);
E_MPa = p(1);                     
E_GPa = E_MPa / 1000;

% R^2
y_fit = polyval(p, strain_linear);
SS_res = sum((stress_linear - y_fit).^2);
SS_tot = sum((stress_linear - mean(stress_linear)).^2);
R2 = 1 - SS_res/SS_tot;

hold on;
plot(strain_linear, polyval(p, strain_linear), 'r-', 'LineWidth', 2);
legend('Data', 'Linear Fit');

fprintf('Modulus of Elasticity: %.2f GPa\n', E_GPa);
fprintf('R^2: %.4f\n', R2);


%ultimate stress
Ultimate_Stress_MPa = max(Stress_MPa);

%failure stress
last_valid = find(Stress_MPa > 0, 1, 'last');
Failure_Stress_MPa = Stress_MPa(last_valid);

fprintf('Ultimate Stress: %.2f MPa\n', Ultimate_Stress_MPa);
fprintf('Failure Stress: %.2f MPa\n', Failure_Stress_MPa);

True_modulus_GPa = 200;
True_Ultimate_Stress_MPa = 440;

%percent error

percentErrorModulus = abs((E_GPa - True_modulus_GPa) / True_modulus_GPa) * 100;
percentErrorUltimate = abs((Ultimate_Stress_MPa - True_Ultimate_Stress_MPa) / True_Ultimate_Stress_MPa) * 100;

fprintf('Percent Error in Modulus of Elasticity: %.2f%%\n', percentErrorModulus);
fprintf('Percent Error in Ultimate Stress: %.2f%%\n', percentErrorUltimate);