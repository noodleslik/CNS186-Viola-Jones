% generated_face_bbs: Takes Caltech WebFaces dataset and generates estimated bounding boxes
%                     for the faces.

clc; clear all; close all;

FEATURES_FILE = 'WebFaces_GroundThruth.txt';
BBS_FILE = 'WebFaces_BBS.txt';
PIC_DIR = 'Caltech_WebFaces/';
SAVE_DIR = 'Faces_Normalized/';

features_file = fopen(FEATURES_FILE, 'r');
bbs_file = fopen(BBS_FILE, 'w');

face_count = 0;
last_filename = '';
image_filename = '';

% Grab every line and generate a face
while(1)
    line = fgetl(features_file);
    if line < 0
        break;
    end

    face_count = face_count + 1
    features = strread(line, '%s');

    image_filename = [PIC_DIR features{1}];

    for i=1:4
        x(i) = str2num(features{2*i});
        y(i) = str2num(features{2*i + 1});
    end

    im_data = imread(image_filename);
    if size(im_data, 3) > 1
        im_data = rgb2gray(im_data);
    end
    y_max = max(y); y_min = min(y); 
    x_max = max(x); x_min = min(x); 
    new_y_min = floor(y_min - ((y_max - y_min)* 1)); % How far the chin is below the mouth.
    new_y_max = ceil(y_max + ((y_max - y_min)* .5)); % How far the top of the forehead is above the eyes.
    new_x_min = floor(x_min - ((x_max - x_min) * .5));
    new_x_max = ceil(x_max + ((x_max - x_min) * .5));

    % Figure out which side is longer and make it a square centered around the nose x-wise
    % and the eyes y-wise.
    if (new_x_max - new_x_min) > (new_y_max - new_y_min)
        new_y_max = new_y_max + ((new_x_max - new_x_min) - (new_y_max - new_y_min));
    else
        new_x_max = new_x_max + ((new_y_max - new_y_min) - (new_x_max - new_x_min));
    end
    y_mean = (new_y_max + new_y_min)/2;
    x_mean = (new_x_max + new_x_min)/2;
    y_diff = ((y(3) + y(2))/2) - y_mean;
    x_diff = x(3) - x_mean;
    new_y_max = new_y_max + y_diff; new_y_min = new_y_min + y_diff;
    new_x_max = new_x_max + x_diff; new_x_min = new_x_min + x_diff; 
    new_y_min = floor(new_y_min); new_y_max = ceil(new_y_max);
    new_x_min = floor(new_x_min); new_x_max = ceil(new_x_max);

    % If the bounding box exits the image, don't use this.
    if new_y_min < 1 || new_x_min < 1 || new_y_max > size(im_data, 1) || new_x_max > size(im_data, 2)
        continue
        face_count = face_count - 1;
    end

    face_data = im_data(new_y_min:new_y_max, new_x_min:new_x_max);
    face_data = imresize(face_data, (24 / (new_y_max - new_y_min))); % Make 24x24

    coords = [num2str(face_count) '.jpg ' features{1} ' '  num2str(new_x_min) ' ' num2str(new_y_min) ...
                          ' '  num2str(new_x_max) ' ' num2str(new_y_max)];
    fprintf(bbs_file, '%s \n', coords);
    imwrite(face_data, [SAVE_DIR num2str(face_count) '.jpg']);
    %figure(1); imshow(im_data); 
    %figure(2); imshow(face_data);
end

fclose('all');
