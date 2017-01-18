------------------------------------------
Date range exposed filter one value filter
------------------------------------------

This Drupal 7 module allows you to add exposed date range filters so you can
filter content which matches between initial and final date. This module works
in standart and in Search API views.



            INITIAL          FINAL
               |--------------|


  CASE MATCH          |


IMPORTANT NOTE: This module is not plug & play. It is meant to be used for
programmers as a base to start with. It assumes that you have a date field
that DOES NOT store an end date. Finally, notice you have to expose the
filter, otherwise the query will not be filtered. 

USAGE: You have to add the filter "Custom: Date range for standart and
search api views (full date)." to your views. When you add it, you will
have to write the machine name of the date field you want to filter by.
