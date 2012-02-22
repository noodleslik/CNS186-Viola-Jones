% generate_negative_dataset.m: Generates negative examples using Caltech256

clc; close all; clear all;

num_per_item = 8;
base_folder = 'Caltech256/256_ObjectCategories';
save_folder = 'Negative_Dataset';
folders = dir(base_folder);
which_image = 1;
for i=3:size(folders, 1)
    current_image_folder = [base_folder '/' folders(i).name];
    images = dir(current_image_folder);
    num_images = size(images, 1);
    which = floor(3 + (num_images - 3) .* rand(num_per_item, 1)); 
    for j=1:size(which, 1)
        %figure(1); hold off; 
        temp = imread([current_image_folder '/' images(which(j)).name]); 
        max_size = min(150, size(temp, 1));
        max_size = min(max_size, size(temp, 2));
        subwindow_size = ceil(10 + (max_size - 10) * rand(1)); subwindow_size = subwindow_size(1);
        subwindow_size = subwindow_size - 1;
        %imshow(temp, 'Border', 'tight'); hold on;
        rand_x = ceil((size(temp, 1) - subwindow_size) .* rand(1));
        rand_y = ceil((size(temp, 2) - subwindow_size) .* rand(1));
        %line([rand_y, rand_y + subwindow_size], [rand_x, rand_x]);
        %line([rand_y + subwindow_size, rand_y + subwindow_size], [rand_x, rand_x + subwindow_size]);
        %line([rand_y, rand_y + subwindow_size], [rand_x + subwindow_size, rand_x + subwindow_size]);
        %line([rand_y, rand_y], [rand_x, rand_x + subwindow_size]);
        subwindow = temp(rand_x(1):rand_x(1)+subwindow_size, rand_y(1):rand_y(1)+subwindow_size, :);
        resized_subwindow = imresize(subwindow, 25/(subwindow_size+1));
        % Make sure it's 25x25
        if size(size(resized_subwindow), 2) == 3
            if max(max(size(resized_subwindow) ~= [25 25 3]))
                resized_subwindow = resized_subwindow(1:25, 1:25, :);
            end
        else
            if max(max(size(resized_subwindow) ~= [25 25]))
                resized_subwindow = resized_subwindow(1:25, 1:25);
            end
        end
        %figure(2); imshow(subwindow); 
        %figure(3); imshow(resized_subwindow);
        imwrite(resized_subwindow, [save_folder '/' num2str(which_image) '.png']); which_image = which_image + 1; 
    end
end
