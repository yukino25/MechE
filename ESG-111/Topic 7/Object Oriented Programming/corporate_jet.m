classdef corporate_jet < aircraft
    properties
        company
        max_passengers
        range_km
    end
    methods
        function obj = corporate_jet(make, model, company, max_passengers, range_km)
            obj = obj@aircraft(make, model);
            obj.company = company;
            obj.max_passengers = max_passengers;
            obj.range_km = range_km;
        end

        function info(obj)
            info@aircraft(obj);
            fprintf('Company: %s, Max Passengers: %d, Range: %d km\n', obj.company, obj.max_passengers, obj.range_km);
        end
    end
end
