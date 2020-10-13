dnl Graphviz package version number, (as distinct from shared library version)
dnl For the minor number: odd => unstable series
dnl                       even => stable series
dnl For the micro number: 0 => in-progress development
dnl                       timestamp => tar-file snapshot or release

dnl uncomment the next 4 lines for development releases, minor version must be odd
dnl m4_define([graphviz_version_major],[2])
dnl m4_define([graphviz_version_minor],[43])
dnl m4_define([graphviz_version_micro],[20190717.1914])
dnl m4_define([graphviz_collection],[development])

dnl uncomment the next 4 lines for stable releases, minor version must be even
m4_define([graphviz_version_major],[2])
m4_define([graphviz_version_minor],[42])
m4_define([graphviz_version_micro],[1])
m4_define([graphviz_collection],[stable])

m4_define([graphviz_version_date],[20190717.1914])
m4_define([graphviz_change_date],["Wed Jul 17 2019"])
m4_define([graphviz_git_date],["2019-07-17 15:14:48 -0400"])
m4_define([graphviz_author_name],["John Ellson"])
m4_define([graphviz_author_email],[john.ellson@gmail.com])
m4_define([graphviz_version_commit],[4397ed7])

