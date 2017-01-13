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
for your content with "field_inici_fi" as its machine name. Also, it assumes
this field DOES NOT store an end date. Finally, notice you have to expose the
filter, otherwise the query will not be filtered.
