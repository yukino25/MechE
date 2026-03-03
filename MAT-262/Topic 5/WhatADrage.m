


%nicholas Schlosser
%21234894

a = 4;
b = 9;
c = 8;
d = 21;

%part 1
syms x
%1A
y1 = (a+1)*x*exp(1)^(-x)*sin((b+1)/x);
y2 = (c+1)*x*acos(x/(d+1));

%1B
P1Bleft = limit(y1,x,0,'left');
P1Bright = limit(y1,x,0,'right');
P1Btwo = limit(y1,x,0);
P1Bdydx = diff(y1,x);
P1Bdyydxx = diff(y1,x,2);

%1C
figure;

fplot(y1,[(c + 1)/10 (d + 25)/5],'-b')
hold on
fplot(P1Bdydx,[(c + 1)/10 (d + 25)/5],'-r')
fplot(P1Bdyydxx,[(c + 1)/10 (d + 25)/5],'-g')
ylim([-20 20])
title('Plot of y1, its first derivative, and its second derivative')
xlabel('x')
ylabel('y')
legend('y1','dy/dx','d^2y/dx^2')
hold off

%1D
figure;
fplot(y2,[-(d+1) (d+1)],'-m')
title('Plot of y2')
xlabel('x')
ylabel('y')
P1D = vpasolve(diff(y2,x) == 0, x, [-(d+1), (d+1)]);

%1E
m = (subs(y2,x,d+1) - subs(y2,x,-(d+1))) / (d+1 - (-(d+1)));
P1E = vpasolve(diff(y2,x) == m, x, [-(d+1), (d+1)]);

%part 2
syms t
assume(t > 0);
v0 = 45 + a;
theta0 = 0.44+(b/50);
y0 = 1;
g = 9.81;
XofT = (v0*cos(theta0))*t;
YofT = -(g/2)*t^2 + (v0*sin(theta0))*t + y0;

%2A  
P2A = double(solve(YofT == 0, t));

%2B 

tmax = P2A;
figure;
fplot(XofT,[0 tmax],'-b')
title('Plot of x vs t')
xlabel('t (s)')
ylabel('x (m)')
figure;
fplot(YofT,[0 tmax],'-r')
title('Plot of y vs t')
xlabel('t (s)')
ylabel('y (m)')
figure;
fplot(XofT,YofT,[0 tmax],'-m')
title('Plot of x vs y')
xlabel('x (m)')
ylabel('y (m)')


%2C

Vt = 42.5 + 0.22*c;
Xdrag = (Vt^2/g)*log((Vt^2 + g*(v0*cos(theta0))*t)/Vt^2);
V = Vt*((v0*sin(theta0) - Vt*tan((g/Vt)*t))/(Vt + (v0*sin(theta0))*tan(g/Vt*t)));
Ydrag = (Vt^2/(2*g))*log(((v0*sin(theta0))^2+Vt^2)/(V^2+Vt^2))+y0;


%2D

figure;
fplot(Xdrag,[0 tmax],'-b')
title('Plot of xdrag vs t')
xlabel('t (s)')
ylabel('xdrag (m)')
figure;
fplot(Ydrag,[0 tmax],'-r')
title('Plot of ydrag vs t')
xlabel('t (s)')
ylabel('ydrag (m)')
figure;
fplot(Xdrag,Ydrag,[0 tmax],'-m')
title('Plot of xdrag vs ydrag')
xlabel('xdrag (m)')
ylabel('ydrag (m)')


%2E

figure;
fplot(XofT,YofT,[0 tmax],'-b')
hold on
fplot(Xdrag,Ydrag,[0 tmax],'-r')
title('Plot of x vs y and xdrag vs ydrag')
xlabel('x (m)')
ylabel('y (m)')
legend('x vs y','xdrag vs ydrag')
