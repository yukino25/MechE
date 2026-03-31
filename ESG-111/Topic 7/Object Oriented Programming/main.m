% Calling script to test aircraft classes

% Base class
a = aircraft('Wright Brothers', 'Flyer');
fprintf('--- Base Aircraft ---\n');
a.info();
fprintf('\n');

% Commercial jet
c = commercial_jet('Boeing', '737', 'Delta Airlines', 150);
fprintf('--- Commercial Jet ---\n');
c.info();
fprintf('\n');

% Seaplane
s = seaplane('De Havilland', 'Beaver', 'Pontoon', 800);
fprintf('--- Seaplane ---\n');
s.info();
fprintf('\n');

% Corporate jet
cj = corporate_jet('Gulfstream', 'G650', 'Acme Corp', 18, 12960);
fprintf('--- Corporate Jet ---\n');
cj.info();
fprintf('\n');

% Quadcopter drone
q = quadcopter('DJI', 'Phantom 4', 6000, 1.3);
fprintf('--- Quadcopter ---\n');
q.info();
