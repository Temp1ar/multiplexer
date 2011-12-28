#
# Regular cron jobs for the mplex package
#
0 4	* * *	root	[ -x /usr/bin/mplex_maintenance ] && /usr/bin/mplex_maintenance
