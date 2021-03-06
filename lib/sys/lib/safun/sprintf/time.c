/*
 *  Copyright (C) 1994 Haijo Schipper (abigail@xs4all.nl)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

# include "config.h"
# include "time.h"

private mixed weekday (int time, int arg) {
  return (arg == 2
              ? member_array (ctime (time) [0 .. 2], WEEK)
              : arg == 1 ? WEEKDAYS [ctime (time) [0 .. 2]]
                         : ctime (time) [0 .. 2]);
}

private mixed month (int time, int arg) {
  return (arg == 2
              ? member_array (ctime (time) [4 .. 6], YEAR)
              : arg == 1 ? MONTHS [ctime (time) [4 .. 6]]
                         : ctime (time) [4 .. 6]);
}

private int day (int time) {
  int d;
  sscanf (ctime (time), "%*s %*s %d %*s %*d", d);
  return (d);
}

private string ttime (int time) {
  string t;
  sscanf (ctime (time), "%*s %*s %*d %s %*d", t);
  return (t);
}

private string date (int time) {
  return (ctime (time) [0 .. 10] + ctime (time) [20 .. 23]);
}

private int year (int time) {
  int y;
  sscanf (ctime (time), "%*s %*s %*d %*s %d", y);
  return (y);
}

private int hour (int time) {
  int h;
  sscanf (ttime (time), "%d:%*d:%*d", h);
  return (h);
}

private int minute (int time) {
  int m;
  sscanf (ttime (time), "%*d:%d:%*d", m);
  return (m);
}

private int second (int time) {
  int s;
  sscanf (ttime (time), "%*d:%*d:%d", s);
  return (s);
}

# ifdef __TIME_ZONE__
/* European rules. DST from 2.00 last Sunday of March until 2.00 last */
/*                 Sunday of September.                               */
private string timezone (int time) {
  int mm, dd, ww;
  mm = month (time, 2) + 1;
  dd = day (time);
  ww = weekday (time, 2);
  return (year (time) < DST_CONVERSION_YEAR || mm < 3 || mm > 9
             ? TZ                                   /* Jan Feb Oct Nov Dec */
             : mm != 3 && mm != 9
                  ? DSTZ                            /* Apr May Jun Jul Aug */
                  : mm == 3 
                       ? dd < 25                    /* Mar */
                            ? TZ                    /* First weeks */
                            : ww
                                ? dd - ww <= 25     /* Not a Sunday */
                                    ? TZ            /* Before last Sunday */
                                    : DSTZ          /* After... */
                                : hour (time) <= 2
                                    ? TZ            /* Before 02.00 */
                                    : DSTZ          /* After... */
                       : dd < 24                    /* Sep */
                            ? DSTZ                  /* First weeks */
                            : ww
                                ? dd - ww <= 24     /* Not a Sunday */
                                    ? DSTZ          /* Before last Sunday */
                                    : TZ            /* After ... */
                                : hour (time) <= 1  /* Last Sunday */
                                    ? DSTZ          /* Before 2.00 */
                                    : hour (time) == 2
                                        ? ctime (time ()) ==
                                          ctime (time () + 3600)
                                            ? DSTZ  /* First time between */
                                                    /* 2.00 and 3.00 */
                                            : TZ    /* Second time... */
                                        : TZ);      /* After 3.00 */
}
# endif

private int leap_year (int year) {
  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

private int day_of_year (int time) {
  int mm, dd, i, doy;
  for (i = 0, mm = month (time, 2); i < mm; doy += YEAR_IN_DAYS [i ++]);
  return (doy + (dd = day (time)) +
          (leap_year (year (time)) && (mm > 1 || mm == 1 && dd == 29) ? 1 : 0));
}


private int week_number (int time, int monday) {
  int doy, wn;
  doy = day_of_year (time) - 1;
  wn  = doy / 7;
  if (weekday (time, 2) <= doy % 7) {wn ++;}
  if (monday && (weekday (time, 2) == 0)) {wn --;}
  return (wn);
}
