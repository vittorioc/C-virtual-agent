clear;

ts = 0.05;

N = 32768;

t=0:ts:(N-1)*ts;

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

figure = figure('XVisual', '0x29 (TrueColor, depth 24, RGB mask 0xff0000 0xff00 0x00ff)');

subplot1 = subplot(2,3,1,'Parent',figure);
box(subplot1,'on');
hold(subplot1,'all');
plot1 = plot(t,p1,'Parent',subplot1);
xlim([0 50]);
ylim([-0.8 0.8]);

subplot2 = subplot(2,3,2,'Parent',figure);
box(subplot2,'on');
hold(subplot2,'all');
plot2 = plot(t,p2,'Parent',subplot2);
xlim([0 50]);
ylim([-0.8 0.8]);

subplot3 = subplot(2,3,3,'Parent',figure);
box(subplot3,'on');
hold(subplot3,'all');
plot3 = plot(t,p3,'Parent',subplot3);
xlim([0 50]);
ylim([-0.8 0.8]);

subplot4 = subplot(2,3,4,'Parent',figure);
box(subplot4,'on');
hold(subplot4,'all');
plot4 = plot(t,p4,'Parent',subplot4);
xlim([0 50]);
ylim([-0.8 0.8]);

subplot5 = subplot(2,3,5,'Parent',figure);
box(subplot5,'on');
hold(subplot5,'all');
plot5 = plot(t,p5,'Parent',subplot5);
xlim([0 50]);
ylim([-0.8 0.8]);

subplot6 = subplot(2,3,6,'Parent',figure);
box(subplot6,'on');
hold(subplot6,'all');
plot6 = plot(t,p6,'Parent',subplot6);
xlim([0 50]);
ylim([-0.8 0.8]);