%Solve both circuit equations, and provide the currents and voltages for each component

%circuit 1

C1_A = [10 0 150;
    0 -300 150;
    1 -1 -1];
C1_B = [5; 0; 0];

C1_X = inv(C1_A)*C1_B;


fprintf("Circuit 1:\n");
fprintf("i1 = %.2f mA\n", vpa(C1_X(1,1))*1000);
fprintf("i2 = %.2f mA\n", vpa(C1_X(2,1))*1000);
fprintf("i3 = %.2f mA\n", vpa(C1_X(3,1))*1000);

fprintf("Voltage:\n");
fprintf("V(10ohm) = %.2f V\n", vpa(C1_X(1,1))*10);
fprintf("V(100ohm) = %.2f V\n", vpa(C1_X(2,1))*100);
fprintf("V(200ohm) = %.2f V\n", vpa(C1_X(2,1))*200);
fprintf("V(150ohm) = %.2f V\n", vpa(C1_X(3,1))*150);



%circuit 2
C2_A = [60 0 20 0 40 0 0;
    0 25 -20 0 0 100 0;
    0 0 0 100 -40 0 70;
    1 -1 -1 0 0 0 0;
    0 0 1 -1 -1 0 0;
    0 1 0 0 0 -1 -1;
    0 0 0 1 0 1 0];
C2_B = [5;0;0;0;0;0;0;];

C2_X = inv(C2_A)*C2_B;

fprintf("Circuit 2:\n");
fprintf("i1 = %.2f mA\n", vpa(C2_X(1,1))*1000);
fprintf("i2 = %.2f mA\n", vpa(C2_X(2,1))*1000);
fprintf("i3 = %.2f mA\n", vpa(C2_X(3,1))*1000);
fprintf("i4 = %.2f mA\n", vpa(C2_X(4,1))*1000);
fprintf("i5 = %.2f mA\n", vpa(C2_X(5,1))*1000);
fprintf("i6 = %.2f mA\n", vpa(C2_X(6,1))*1000);
fprintf("i7 = %.2f mA\n", vpa(C2_X(7,1))*1000);


fprintf("Voltage:\n");
fprintf("V(60ohm) = %.2f V\n", vpa(C2_X(1,1))*60);
fprintf("V(20ohm) = %.2f V\n", vpa(C2_X(3,1))*20);
fprintf("V(25ohm) = %.2f V\n", vpa(C2_X(2,1))*25);
fprintf("V(75ohm) = %.2f V\n", vpa(C2_X(7,1))*75);
fprintf("V(40ohm) = %.2f V\n", vpa(C2_X(5,1))*40);
fprintf("V(100ohm) = %.2f V\n", vpa(C2_X(4,1)+C2_X(6,1))*100);


%circuit 3
C3_A = [10 0 150 0 150;
    0 300 -150 0 0;
    0 0 0 700 150;
    1 -1 -1 0 0;
    0 0 0 -1 1];

C3_B =[5;0;0;0;0];

C3_X = inv(C3_A)*C3_B;

fprintf("Circuit 3:\n");
fprintf("i1 = %.2f mA\n", vpa(C3_X(1,1))*1000);
fprintf("i2 = %.2f mA\n", vpa(C3_X(2,1))*1000);
fprintf("i3 = %.2f mA\n", vpa(C3_X(3,1))*1000);
fprintf("i4 = %.2f mA\n", vpa(C3_X(4,1))*1000);
fprintf("i5 = %.2f mA\n", vpa(C3_X(5,1))*1000);


fprintf("Voltage:\n");
fprintf("V(10ohm) = %.2f V\n", vpa(C3_X(1,1))*10);
fprintf("V(100ohm) = %.2f V\n", vpa(C3_X(1,1))*100);
fprintf("V(200ohm) = %.2f V\n", vpa(C3_X(2,1))*200);
fprintf("V(500ohm) = %.2f V\n", vpa(C3_X(4,1))*500);
fprintf("V(150ohm) = %.2f V\n", vpa(C3_X(3,1))*150);
fprintf("V(200ohm) = %.2f V\n", vpa(C3_X(4,1)+C3_X(2,1))*200);
fprintf("V(150ohm) = %.2f V\n", vpa(C3_X(5,1))*150);
