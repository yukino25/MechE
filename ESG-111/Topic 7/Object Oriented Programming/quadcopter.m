classdef quadcopter < aircraft
    properties
        max_altitude_m
        payload_kg
    end
    methods
        function obj = quadcopter(make, model, max_altitude_m, payload_kg)
            obj = obj@aircraft(make, model);
            obj.max_altitude_m = max_altitude_m;
            obj.payload_kg = payload_kg;
        end

        function info(obj)
            info@aircraft(obj);
            fprintf('Max Altitude: %d m, Payload: %.1f kg\n', obj.max_altitude_m, obj.payload_kg);
        end
    end
end
