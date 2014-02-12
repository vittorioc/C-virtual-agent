clear;

V = 6;

ts = 0.05; 

N = 32768;

t0 = 0;

dmax = 2*(V-1);

fs = 1/(ts*N);

f = (0:N-1)*fs*2*pi;

f_half = f(1:N/2);

tolerance = 0.00001;

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

figure1 = figure('XVisual', '0x29 (TrueColor, depth 24, RGB mask 0xff0000 0xff00 0x00ff)');

E1 = fft(e1);
E1_half = E1(1:N/2);

subplot1 = subplot(2,6,1,'Parent',figure1);
box(subplot1,'on');
hold(subplot1,'all');
E1M = abs(E1_half)*2/N; 
E1M(1) = E1M(1)/2;
plot(f_half,E1M,'Parent',subplot1);
xlim([0 dmax]);
subplot7 = subplot(2,6,7,'Parent',figure1);
box(subplot7,'on');
hold(subplot7,'all');
E1P1 = ceil(abs(E1_half) - tolerance);
E1P2 = round(E1P1 ./ (E1P1+1));
E1P = angle(E1_half).*E1P2;
stem(f_half,E1P,'Parent',subplot7);
xlim([0 dmax]);

E2 = fft(e2);
E2_half = E2(1:N/2);

subplot2 = subplot(2,6,2,'Parent',figure1);
box(subplot2,'on');
hold(subplot2,'all');
E2M = abs(E2_half)*2/N; 
E2M(1) = E2M(1)/2;
plot(f_half,E2M,'Parent',subplot2);
xlim([0 dmax]);
subplot8 = subplot(2,6,8,'Parent',figure1);
box(subplot8,'on');
hold(subplot8,'all');
E2P1 = ceil(abs(E2_half) - tolerance);
E2P2 = round(E2P1 ./ (E2P1+1));
E2P = angle(E2_half).*E2P2;
stem(f_half,E2P,'Parent',subplot8);
xlim([0 dmax]);

E3 = fft(e3);
E3_half = E3(1:N/2);

subplot3 = subplot(2,6,3,'Parent',figure1);
box(subplot3,'on');
hold(subplot3,'all');
E3M = abs(E3_half)*2/N; 
E3M(1) = E3M(1)/2;
plot(f_half,E3M,'Parent',subplot3);
xlim([0 dmax]);
subplot9 = subplot(2,6,9,'Parent',figure1);
box(subplot9,'on');
hold(subplot9,'all');
E3P1 = ceil(abs(E3_half) - tolerance);
E3P2 = round(E3P1 ./ (E3P1+1));
E3P = angle(E3_half).*E3P2;
stem(f_half,E3P,'Parent',subplot9);
xlim([0 dmax]);

E4 = fft(e4);
E4_half = E4(1:N/2);

subplot4 = subplot(2,6,4,'Parent',figure1);
box(subplot4,'on');
hold(subplot4,'all');
E4M = abs(E4_half)*2/N; 
E4M(1) = E4M(1)/2;
plot(f_half,E4M,'Parent',subplot4);
xlim([0 dmax]);
subplot10 = subplot(2,6,10,'Parent',figure1);
box(subplot10,'on');
hold(subplot10,'all');
E4P1 = ceil(abs(E4_half) - tolerance);
E4P2 = round(E4P1 ./ (E4P1+1));
E4P = angle(E4_half).*E4P2;
stem(f_half,E4P,'Parent',subplot10);
xlim([0 dmax]);

E5 = fft(e5);
E5_half = E5(1:N/2);

subplot5 = subplot(2,6,5,'Parent',figure1);
box(subplot5,'on');
hold(subplot5,'all');
E5M = abs(E5_half)*2/N; 
E5M(1) = E5M(1)/2;
plot(f_half,E5M,'Parent',subplot5);
xlim([0 dmax]);
subplot11 = subplot(2,6,11,'Parent',figure1);
box(subplot11,'on');
hold(subplot11,'all');
E5P1 = ceil(abs(E5_half) - tolerance);
E5P2 = round(E5P1 ./ (E5P1+1));
E5P = angle(E5_half).*E5P2;
stem(f_half,E5P,'Parent',subplot11);
xlim([0 dmax]);

E6 = fft(e6);
E6_half = E6(1:N/2);

subplot6 = subplot(2,6,6,'Parent',figure1);
box(subplot6,'on');
hold(subplot6,'all');
E6M = abs(E6_half)*2/N; 
E6M(1) = E6M(1)/2;
plot(f_half,E6M,'Parent',subplot6);
xlim([0 dmax]);
subplot12 = subplot(2,6,12,'Parent',figure1);
box(subplot12,'on');
hold(subplot12,'all');
E6P1 = ceil(abs(E6_half) - tolerance);
E6P2 = round(E6P1 ./ (E6P1+1));
E6P = angle(E6_half).*E6P2;
stem(f_half,E6P,'Parent',subplot12);
xlim([0 dmax]);