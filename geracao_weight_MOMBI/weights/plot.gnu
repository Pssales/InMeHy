set grid
set key
plot [1:2] "<awk '$1 !~ /#/ { for(i = 1; i <= NF; i++) print i,$i; print z}' output/weight_02D_100.sld" u 1:2:xtic(1) w linesp linecolor 5 title 'output/weight_02D_100.sld'
pause -1
plot 'output/weight_02D_100.sld' pt 7 lc 3 title 'output/weight_02D_100.sld'
pause -1
