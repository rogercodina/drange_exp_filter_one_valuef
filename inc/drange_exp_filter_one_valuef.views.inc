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

  $fname = drange_exp_filter_one_valuef_get_filter_field_name($view);

  if (!empty($view->exposed_raw_input['fulld_from']) || !empty($view->exposed_raw_input['fulld_to'])) {

    $case = array();
    $vars_query = array();

    if (!empty($view->exposed_raw_input['fulld_from'])) {
      $fulld_from = $view->exposed_raw_input['fulld_from'];
      $ini = format_date(strtotime($fulld_from . ' 00:00:00'), 'custom', 'Ymd');

      // Build query.
      $case[] = "DATE_FORMAT(field_data_{$fname}.{$fname}_value, '%Y%m%d') >= :ini";
      $vars_query[':ini'] = $ini;
    }

    if (!empty($view->exposed_raw_input['fulld_to'])) {
      $fulld_to = $view->exposed_raw_input['fulld_to'];
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

/**
 * Implements hook_search_api_views_query_alter().
 *
 * Unica manera de modificar la query si el filtre estar exposat (Search API).
 */
function drange_exp_filter_one_valuef_search_api_views_query_alter(&$view, &$query) {

  $fname = drange_exp_filter_one_valuef_get_filter_field_name($view);

  /*
   * IMPORTANT: Use field names that appear in admin/config/search/search_api/index/[INDEX_NAME]/fields
   * NOTE: Search API stores dates as timestamps
   */
  if (!empty($view->exposed_raw_input['fulld_from']) || !empty($view->exposed_raw_input['fulld_to'])) {

    $query->addField($fname);
    $main_filter = $query->createFilter('AND');

    if (!empty($view->exposed_raw_input['fulld_from'])) {
      $fulld_from = $view->exposed_raw_input['fulld_from'];
      $ini = strtotime($fulld_from . ' 00:00:00');

      // Build query.
      $subfilter1 = $query->createFilter('AND');
      $subfilter1->condition($fname, $ini , '>=');
      $main_filter->filter($subfilter1);
    }

    if (!empty($view->exposed_raw_input['fulld_to'])) {
      $fulld_to = $view->exposed_raw_input['fulld_to'];
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

/**
 * Get filter_field_name option.
 *
 * param $view object;
 *
 * @return string Field name.
 */
function drange_exp_filter_one_valuef_get_filter_field_name($view) {
  $field_name = 'field_date';

  foreach($view->display_handler->handlers['filter'] as $filter) {
    if (is_object($filter) && is_a($filter, 'drange_exp_filter_one_valuef_handler_filter_fulld_sapi')) {
      if (!empty($filter->options['filter_field_name'])) {
        $field_name = $filter->options['filter_field_name'];
      }
    }
  }

  return $field_name;
}
