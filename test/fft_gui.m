function fftgui

% FFTGUI Plot fft and change parameter dynamically.

ts = 0.05;

N = 32768;

%F = 6; %: valida per visualizzare correttamente frequenze sotto i 64Hz (esclusa)
F = 1/ts;

fs = F/N;

T = 1/fs;

t0 = 0;

t = t0 + (0:N-1)*ts;
f = (0:N-1)*fs*2*pi;

str_pop1 = {'cos', 'agent_1.log', 'agent_2.log', 'agent_3.log', 'agent_4.log', 'agent_5.log', 'agent_6.log'};

str_pop2 = {'Rectangular', 'Bartlett', 'Bartlett-Hann', 'Blackman', 'Blackman-Harris', 'Bohman', 'Chebyshev', 'Flat-Top', 'Gaussian', 'Hamming', 'Hann', 'Nuttall', 'Parzen', 'Taylor', 'Triangular'};

str_pop3 = {'1', '2', '4', '8', '16', '32', '64', '128', '256', '512', '1024', '2048', '4096'};

str_pop4 = {'1', '2', '4', '8', '16', '32', '64', '128', '256', '512', '1024', '2048', '4096'};

val_pop1 = 1;
val_pop2 = 1;
val_pop3 = 1;
val_pop4 = 1;

win_min = 32;
win_len = 32;

figure1 = figure('Visible', 'on', 'Name', 'FFT GUI', 'Position', [80,100,1200,480], 'XVisual', '0x29 (TrueColor, depth 24, RGB mask 0xff0000 0xff00 0x00ff)');

a1 = axes('Parent', figure1, 'Units', 'normalized', 'Position', [.04 .4 .56 .5], 'box', 'on');

a2 = axes('Parent', figure1, 'Units', 'normalized', 'Position', [0.64 .55 .32 .35], 'box', 'on');

a3 = axes('Parent', figure1, 'Units', 'normalized', 'Position', [0.64 .1 .32 .35], 'box', 'on');

pop1 = uicontrol('Parent', figure1, 'Units', 'normalized', 'Position', [0.04 0.13 0.10 0.03], 'BackgroundColor', [  .8   .8   .8], 'Style', 'popupmenu', 'String', str_pop1, 'Callback',{@pop1_Callback});

pop2 = uicontrol('Parent', figure1, 'Units', 'normalized', 'Position', [0.16 0.13 0.10 0.03], 'BackgroundColor', [  .8   .8   .8], 'Style', 'popupmenu', 'String', str_pop2, 'Callback',{@pop2_Callback});

txt1 = uicontrol('Parent', figure1, 'Units', 'normalized', 'Position', [0.00 0.27 0.04 0.03], 'BackgroundColor', [  .8   .8   .8], 'Style', 'text', 'String', 't0:');

%eth1 = uicontrol('Parent', figure1, 'Units', 'normalized', 'Position', [0.04 0.27 0.05 0.03], 'BackgroundColor', [   1    1    1], 'Style', 'edit', 'String', num2str(win_min), 'Callback',{@eth1_Callback});

slr1 = uicontrol('Parent', figure1, 'Units', 'normalized', 'Position', [0.04 0.27 0.56 0.03], 'BackgroundColor', [   1    1    1], 'Style', 'slider', 'Min', 0, 'Max', N, 'Value', win_min, 'SliderStep', [1/N 1/N], 'Callback', @slider1_Callback);
  
%pop3 = uicontrol('Parent', figure1, 'Units', 'normalized', 'Position', [0.55 0.27 0.05 0.03], 'BackgroundColor', [.729 .831 .957], 'Style', 'popupmenu', 'String', str_pop3, 'Callback',{@pop3_Callback});

txt2 = uicontrol('Parent', figure1, 'Units', 'normalized', 'Position', [0.00 0.20 0.04 0.03], 'BackgroundColor', [  .8   .8   .8], 'Style', 'text', 'String', 'width:');

%eth2 = uicontrol('Parent', figure1, 'Units', 'normalized', 'Position', [0.04 0.20 0.05 0.03], 'BackgroundColor', [   1    1    1], 'Style', 'edit', 'String', num2str(win_len), 'Callback',{@eth2_Callback});

slr2 = uicontrol('Parent', figure1, 'Units', 'normalized', 'Position', [0.04 0.20 0.56 0.03], 'BackgroundColor', [   1    1    1], 'Style', 'slider', 'Min', 0, 'Max', N, 'Value', win_len, 'SliderStep', [1/N 1/N], 'Callback', @slider2_Callback);

%pop4 = uicontrol('Parent', figure1, 'Units', 'normalized', 'Position', [0.55 0.20 0.05 0.03], 'BackgroundColor', [.729 .831 .957], 'Style', 'popupmenu', 'String', str_pop4, 'Callback',{@pop4_Callback});

disegna;

    function eth1_Callback(hObject,eventdata)
    
      val_eth1 = str2double(get(hObject,'String'));
      
      if (val_eth1 > 0) && ((val_eth1 + win_len - 1) <= N),
         win_min = val_eth1; 
         set(slr1,'Value',val_eth1),
      end;
      
      disegna;
      
    end
    
    function eth2_Callback(hObject,eventdata)
    
      val_eth2 = str2double(get(hObject,'String'));
      
      if (val_eth2 > 0) && ((win_min + val_eth2 - 1) <= N), 
         win_len = val_eth2; 
         set(slr2,'Value',val_eth2);
      end;
      
      disegna;
      
    end
    
    function slider1_Callback(hObject,eventdata)
      
      val_slider1 = get(hObject,'Value');
      
      if (val_slider1 > 0) && ((val_slider1 + win_len - 1) <= N),
          win_min = val_slider1; 
          ;
%          set(eth1,'String',num2str(win_min)),
      end;
       
      disegna;
      
    end

    function slider2_Callback(hObject,eventdata)
      
      val_slider2 = get(hObject,'Value');
      
      if (val_slider2 > 0) && ((win_min + val_slider2 - 1) <= N), 
          win_len = val_slider2; 
          ;
%         set(eth2,'String',num2str(win_len));
      end;
      
      disegna;
      
    end

    function pop1_Callback(source,eventdata)
        
        str_pop1 = get(source, 'String');
        val_pop1 = get(source, 'Value');
        
        disegna;
        
    end

    function pop2_Callback(source,eventdata)
        
        str_pop2 = get(source, 'String');
        val_pop2 = get(source, 'Value');
        
        disegna;
        
    end
    
    function pop3_Callback(source,eventdata)
        
        val_pop3 = get(source, 'Value');
        
        set(slr1,'SliderStep',[1/N 2^(val_pop3-1)/N]);
        
        disegna;
        
    end
    
    function pop4_Callback(source,eventdata)
        
        val_pop4 = get(source, 'Value');
        
        set(slr2,'SliderStep',[1/N 2^(val_pop4-1)/N]);
        
        disegna;
    
    end
    
    function disegna
        
        switch str_pop1{val_pop1};
            case 'cos'
                x = cos(t + pi/4);
            otherwise
                x = dlmread(str_pop1{val_pop1});
                x(:,2)=[];
                x = x';
        end
        
        w(1:N) = 0;
        
        win_max = win_min + win_len - 1;
        
        switch str_pop2{val_pop2};
        
            case 'Rectangular'
                w(win_min:win_max) = rectwin(win_len)';
            case 'Bartlett'
                w(win_min:win_max) = bartlett(win_len)';
            case 'Bartlett-Hann'
                w(win_min:win_max) = barthannwin(win_len)';
            case 'Blackman'
                w(win_min:win_max) = blackman(win_len)';
            case 'Blackman-Harris'
                w(win_min:win_max) = blackmanharris(win_len)';
            case 'Bohman'
                w(win_min:win_max) = bohmanwin(win_len)';
            case 'Chebyshev'
                w(win_min:win_max) = chebwin(win_len)';
            case 'Flat-Top'
                w(win_min:win_max) = flattopwin(win_len)';
            case 'Gaussian'
                w(win_min:win_max) = gausswin(win_len)';
            case 'Hamming'
                w(win_min:win_max) = hamming(win_len)';
            case 'Hann'
                w(win_min:win_max) = hann(win_len)';
            case 'Nuttall'
                w(win_min:win_max) = nuttallwin(win_len)';
            case 'Parzen'
                w(win_min:win_max) = parzenwin(win_len)';
            case 'Taylor'
                w(win_min:win_max) = taylorwin(win_len)';
            case 'Triangular'
                w(win_min:win_max) = triang(win_len)';
             
        end
        
        xw = x.*w;
        
        X = fft(xw);
        
        f_half = f(1:N/2);
        X_half = X(1:N/2);
        
        X1 = abs(X_half)*2/N;
        X1(1) = X1(1)/2;
        
        tolerance = 0.00001;
        X2 = ceil(abs(X_half) - tolerance);
        X3 = round(X2 ./ (X2+1));
        X4 = angle(X_half).*X3;
        
        % set(a1, 'xlim', [t0 t0+T]);
        
        plot(t, x, 'green', 'Parent', a1);
        hold(a1, 'all');
        set(get(a1,'title'), 'String', 'Time plot');
        set(get(a1,'xlabel'), 'String', 'time (sec)');
        set(get(a1,'ylabel'), 'String', 'amplitude');
        plot(t, xw, 'blue', 'Parent', a1);
        plot(t, w, 'black', 'Parent', a1);
        hold(a1, 'off');
        
        plot(f_half, X1, 'red', 'Parent', a2);   
        hold(a2, 'all');
        set(get(a2,'title'), 'String', 'Frequency magnitude plot');
        set(a2,'YAxisLocation', 'right');
        set(get(a2,'ylabel'), 'String', 'magnitude');
        set(a2, 'xlim', [0 6]);
        hold(a2, 'off');
        
        stem(f_half, X4, 'red', 'Parent', a3);
        hold(a3, 'all');
        set(get(a3,'title'), 'String', 'Phase angle plot');
        set(get(a3,'xlabel'), 'String', 'frequency (Hz)');
        set(a3,'YAxisLocation', 'right');
        set(get(a3,'ylabel'), 'String', 'phase angle');
        set(a3, 'xlim', [0 6]);
        hold(a3, 'off');
        
        linkaxes([a2 a3], 'x');  
        
    end

end
