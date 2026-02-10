%nicholas schlosser
%21234894
%part 1
a=4;
b=9;
c=8;
d=21;

P1B = (a+d)*log(abs(c-15))+sin(log10(factorial(b)))-(b+c)*tan(a/(d+1))

P1C = exp(1)^(sqrt(a*b*c+3))/pi^2-atan(d)^(1/3)

P1D = (b+4)*cos(log2(c+d+1))-(asin(sqrt(2)/2)*acos(d/27))
%part 2
M1 = [a+1 b;
    c d+1];
M2 = [-a 2*b+1 -3*c;
    a+b c-d a+c];

M3 = [a^2 -a^2;
    -b^2 b^2;
    c^2 -c^2];

M4 = [a*b 0 c*d;
    -1 b*c 1;
    a*c 0 a*d];


P2B = 8*M1*M2-4*M2
P2C = -3*M4^2+(7*M3*M2)
P2D = (5*M2*M3).*M1^3
P2E = (6*M4*M3)./(M3.^2)


%part 3
P3AM = [5 10, -9;
        9 22, -4];

REF= rref(P3AM);

P3A = [REF(1,3);
        REF(2,3)]

P3BM = [5 10;
        9 22];
P3BV = [-9;
        -4];

P3B = inv(P3BM)*P3BV

P3C1 = [5 10];
P3C2 = [9 22];

P3C = P3C1 * (P3C2)'

syms z
P3DM = [6 11;
      10 23];
P3D = solve(z^2-trace(P3DM)*z+det(P3DM)==0,z)

P3EM = [6 11;
        10 23];
P3E = eig(P3EM)