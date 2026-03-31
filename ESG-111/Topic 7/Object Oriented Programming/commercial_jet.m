classdef commercial_jet < aircraft
    properties
        num_passengers
        airline
    end
    methods
        function obj = commercial_jet(make, model, airline, num_passengers)
            obj = obj@aircraft(make, model);  % call parent constructor
            obj.airline = airline;
            obj.num_passengers = num_passengers;
        end
        
        function info(obj)
            info@aircraft(obj);  % call parent info
            fprintf('Airline: %s, Passengers: %d\n', obj.airline, obj.num_passengers);
        end
    end
end
