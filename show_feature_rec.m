function show_feature_rec(x1, y1, x2, y2, type)
xdiff = x2-x1;
ydiff = y2-y1;
rectangle('Position', [x1, y1, xdiff, ydiff], 'EdgeColor', 'k', 'FaceColor', 'k');

if type == 0 % Two horiz
rectangle('Position', [x2, y1, xdiff, ydiff], 'EdgeColor', 'w', 'FaceColor', 'w');

elseif type == 1 % Two vert
rectangle('Position', [x1, y2, xdiff, ydiff], 'EdgeColor', 'w', 'FaceColor', 'w');

elseif type == 2 % Three horiz
rectangle('Position', [x2, y1, xdiff, ydiff], 'EdgeColor', 'w', 'FaceColor', 'w');
rectangle('Position', [x2+xdiff, y1, xdiff, ydiff], 'EdgeColor', 'k', 'FaceColor', 'k');

elseif type == 3 % Three vert
rectangle('Position', [x1, y2, xdiff, ydiff], 'EdgeColor', 'w', 'FaceColor', 'w');
rectangle('Position', [x1, y2+ydiff, xdiff, ydiff], 'EdgeColor', 'k', 'FaceColor', 'k');

elseif type == 4 % Four
rectangle('Position', [x2, y1, xdiff, ydiff], 'EdgeColor', 'w', 'FaceColor', 'w');
rectangle('Position', [x1, y2, xdiff, ydiff], 'EdgeColor', 'w', 'FaceColor', 'w');
rectangle('Position', [x2, y2, xdiff, ydiff], 'EdgeColor', 'k', 'FaceColor', 'k');

end


end
