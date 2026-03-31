classdef aircraft
    properties
        make
        model
    end
    methods
        function obj = aircraft(make, model)
            obj.make = make;
            obj.model = model;
        end

        function info(obj)
            fprintf('Aircraft Make: %s, Model: %s\n', obj.make, obj.model);
        end
    end
end
