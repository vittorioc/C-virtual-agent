clear;

ts = 0.05; 

N = 32768;

t=(0:ts:(N-1)*ts)';


e1 = dlmread('agent_1.log');
e2 = dlmread('agent_2.log');
e3 = dlmread('agent_3.log');
e4 = dlmread('agent_4.log');
e5 = dlmread('agent_5.log');
e6 = dlmread('agent_6.log');

e1(:,2)=[];
e2(:,2)=[];
e3(:,2)=[];
e4(:,2)=[];
e5(:,2)=[];
e6(:,2)=[];

p1 = e1';
p2 = e2';
p3 = e3';
p4 = e4';
p5 = e5';
p6 = e6';

M = [0 1 0 0 1 0;
1 0 1 0 0 1;
0 1 0 1 1 1;
0 0 1 0 0 0; 
1 0 1 0 0 0; 
0 1 1 0 0 0];

x0 = [0 0 0 0 0 0 1 0 0 0 0 0]';

D = diag(sum(M));

L = D - M;

Z = zeros(length(M));

A = [Z L; -L Z];

B = zeros(length(A),1);

C = eye(length(A));

D = zeros(length(A),1);

sys_mimo = ss(A,B,C,D);

[y,t,x] = initial(sys_mimo, x0, t);

figure = figure('XVisual', '0x29 (TrueColor, depth 24, RGB mask 0xff0000 0xff00 0x00ff)');

subplot1 = subplot(2,3,1,'Parent',figure);
box(subplot1,'on');
hold(subplot1,'all');
plot1 = plot(t',p1-y(:,1)','Parent',subplot1);

subplot2 = subplot(2,3,2,'Parent',figure);
box(subplot2,'on');
hold(subplot2,'all');
plot2 = plot(t',p2-y(:,2)','Parent',subplot2);

subplot3 = subplot(2,3,3,'Parent',figure);
box(subplot3,'on');
hold(subplot3,'all');
plot3 = plot(t',p3-y(:,3)','Parent',subplot3);

subplot4 = subplot(2,3,4,'Parent',figure);
box(subplot4,'on');
hold(subplot4,'all');
plot4 = plot(t',p4-y(:,4)','Parent',subplot4);

subplot5 = subplot(2,3,5,'Parent',figure);
box(subplot5,'on');
hold(subplot5,'all');
plot5 = plot(t',p5-y(:,5)','Parent',subplot5);

subplot6 = subplot(2,3,6,'Parent',figure);
box(subplot6,'on');
hold(subplot6,'all');
plot6 = plot(t',p6-y(:,6)','Parent',subplot6);
