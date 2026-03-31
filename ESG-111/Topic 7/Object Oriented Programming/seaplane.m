classdef seaplane < aircraft
    properties
        float_type
        water_range_km
    end
    methods
        function obj = seaplane(make, model, float_type, water_range_km)
            obj = obj@aircraft(make, model);
            obj.float_type = float_type;
            obj.water_range_km = water_range_km;
        end

        function info(obj)
            info@aircraft(obj);
            fprintf('Float Type: %s, Water Range: %d km\n', obj.float_type, obj.water_range_km);
        end
    end
end
