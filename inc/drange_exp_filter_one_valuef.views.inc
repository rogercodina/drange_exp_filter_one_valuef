<?php

/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_query_alter().
 *
 * Unica manera de modificar la query si el filtre estar exposat (vistes normals).
 */
function drange_exp_filter_one_valuef_views_query_alter(&$view, &$query) {

  $filters = drange_exp_filter_one_valuef_get_filters($view);

  // Process each filter of our type.
  foreach ($filters as $key => $filter) {
    $fname = $filter['fname'];
    $opt_field_from = 'fulld_from_' . $filter['id'];
    $opt_field_to = 'fulld_to_' . $filter['id'];

    if (!empty($view->exposed_raw_input[$opt_field_from]) || !empty($view->exposed_raw_input[$opt_field_to])) {
      $case = array();
      $vars_query = array();

      if (!empty($view->exposed_raw_input[$opt_field_from])) {
        $fulld_from = $view->exposed_raw_input[$opt_field_from];
        $ini = format_date(strtotime($fulld_from . ' 00:00:00'), 'custom', 'Ymd');

        // Build query.
        $case[] = "DATE_FORMAT(field_data_{$fname}.{$fname}_value, '%Y%m%d') >= :ini";
        $vars_query[':ini'] = $ini;
      }

      if (!empty($view->exposed_raw_input[$opt_field_to])) {
        $fulld_to = $view->exposed_raw_input[$opt_field_to];
        $fin = format_date(strtotime($fulld_to . ' 23:59:59'), 'custom', 'Ymd');

        // Build query.
        $case[] = "DATE_FORMAT(field_data_{$fname}.{$fname}_value, '%Y%m%d') <= :fin";
        $vars_query[':fin'] = $fin;
      }

      $case = '(' . implode(' AND ', $case) . ')';
      $query->add_where_expression(1, $case, $vars_query);
      $query->add_table("field_data_$fname");

      //dpm($query);
    }
  }
}

/**
 * Implements hook_search_api_views_query_alter().
 *
 * Unica manera de modificar la query si el filtre estar exposat (Search API).
 */
function drange_exp_filter_one_valuef_search_api_views_query_alter(&$view, &$query) {

  $filters = drange_exp_filter_one_valuef_get_filters($view);

  // Process each filter of our type.
  foreach ($filters as $key => $filter) {
    $fname = $filter['fname'];
    $opt_field_from = 'fulld_from_' . $filter['id'];
    $opt_field_to = 'fulld_to_' . $filter['id'];

    /*
     * IMPORTANT: Use field names that appear in admin/config/search/search_api/index/[INDEX_NAME]/fields
     * NOTE: Search API stores dates as timestamps
     */
    if (!empty($view->exposed_raw_input[$opt_field_from]) || !empty($view->exposed_raw_input[$opt_field_to])) {

      $query->addField($fname);
      $main_filter = $query->createFilter('AND');

      if (!empty($view->exposed_raw_input[$opt_field_from])) {
        $fulld_from = $view->exposed_raw_input[$opt_field_from];
        $ini = strtotime($fulld_from . ' 00:00:00');

        // Build query.
        $subfilter1 = $query->createFilter('AND');
        $subfilter1->condition($fname, $ini , '>=');
        $main_filter->filter($subfilter1);
      }

      if (!empty($view->exposed_raw_input[$opt_field_to])) {
        $fulld_to = $view->exposed_raw_input[$opt_field_to];
        $fin = strtotime($fulld_to . ' 23:59:59');

        // Build query.
        $subfilter2 = $query->createFilter('AND');
        $subfilter2->condition($fname, $fin , '<=');
        $main_filter->filter($subfilter2);
      }

      $query->filter($main_filter);

      //dpm($query);
    }
  }
}

/**
 * Get list of fields to filter.
 *
 * For every field, we are given the field ID and field name that we should filter.
 *
 * param $view object;
 *
 * @return arrays list of field to filter.
 */
function drange_exp_filter_one_valuef_get_filters($view) {
  $filters = array();

  foreach ($view->display_handler->handlers['filter'] as $filter) {
    if (is_object($filter) && is_a($filter, 'drange_exp_filter_one_valuef_handler_filter_fulld_sapi')) {
      $f = array();
      $f['id'] = $filter->options['id'];
      if (!empty($filter->options['filter_field_name_' . $f['id']])) {
        $f['fname'] = $filter->options['filter_field_name_' . $f['id']];
      }
      $filters[] = $f;
    }
  }

  return $filters;
}
