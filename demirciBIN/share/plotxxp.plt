set terminal postscript eps enhanced color
set output "./share/xxp.eps"
set title "Phase space"
set yrange [-300:300]
set ylabel "Xp (mrad)"
set xrange [-2:2]
set xlabel "X (mm)"
 unset key
plot "./share/x_xp.txt" u 1:2 w p