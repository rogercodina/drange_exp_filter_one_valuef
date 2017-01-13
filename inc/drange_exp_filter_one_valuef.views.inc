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
  if (!empty($view->exposed_raw_input['fulld_from']) || !empty($view->exposed_raw_input['fulld_to'])) {

    $case = array();
    $vars_query = array();

    if (!empty($view->exposed_raw_input['fulld_from'])) {
      $fulld_from = $view->exposed_raw_input['fulld_from'];
      $fulld_from = strtotime($fulld_from) + (60 * 60);
      $ini = date('Ymd', $fulld_from);

      // Build query.
      $case[] = "DATE_FORMAT(field_data_field_inici_fi.field_inici_fi_value, '%Y%m%d') >= :ini";
      $vars_query[':ini'] = $ini;
    }
    
    if (!empty($view->exposed_raw_input['fulld_to'])) {
      $fulld_to = $view->exposed_raw_input['fulld_to'];
      $fulld_to = strtotime($fulld_to) + (60 * 60);
      $fin = date('Ymd', $fulld_to);

      // Build query.
      $case[] = "DATE_FORMAT(field_data_field_inici_fi.field_inici_fi_value, '%Y%m%d') <= :fin";
      $vars_query[':fin'] = $fin;
    }

    $case = '(' . implode(' AND ', $case) . ')';
    $query->add_where_expression(1, $case, $vars_query);
    $query->add_table('field_data_field_inici_fi');
      
    //dpm($query);
  }
}

/**
 * Implements hook_search_api_views_query_alter().
 *
 * Unica manera de modificar la query si el filtre estar exposat (Search API).
 */
function drange_exp_filter_one_valuef_search_api_views_query_alter(&$view, &$query) {
  /*
   * IMPORTANT: Use field names that appear in admin/config/search/search_api/index/[INDEX_NAME]/fields
   * NOTE: Search API stores dates as timestamps
   */
  if (!empty($view->exposed_raw_input['fulld_from']) || !empty($view->exposed_raw_input['fulld_to'])) {

    $query->addField('field_inici_fi');
    $main_filter = $query->createFilter('AND');

    if (!empty($view->exposed_raw_input['fulld_from'])) {
      $fulld_from = $view->exposed_raw_input['fulld_from'];
      $ini = strtotime($fulld_from) + (60 * 60);

      // Build query.
      $subfilter1 = $query->createFilter('AND');
      $subfilter1->condition('field_inici_fi', $ini , '>=');
      $main_filter->filter($subfilter1);
    }
    
    if (!empty($view->exposed_raw_input['fulld_to'])) {
      $fulld_to = $view->exposed_raw_input['fulld_to'];
      $fin = strtotime($fulld_to) + (60 * 60);

      // Build query.
      $subfilter2 = $query->createFilter('AND');
      $subfilter2->condition('field_inici_fi', $fin , '<=');
      $main_filter->filter($subfilter2);
    }

    $query->filter($main_filter);

    //dpm($query);
  }
}
