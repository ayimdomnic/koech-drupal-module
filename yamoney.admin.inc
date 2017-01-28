<?php

/**
 * @return array
 */
function money_admin_settings() {
  $form = array();

  // General settings
  $form['money_all'] = array(
    '#type' => 'fieldset',
    '#title' => t('Base options'),
  );
  $form['money_all']['money_ip'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed IPs for callbacks'),
    '#default_value' => variable_get('money_ip', '0.0.0.0'),
    '#description' => t('The list of IP addresses which has access to payment callbacks. One per line.<br/>0.0.0.0 means allow from all.')
  );
  $form['money_all']['money_mode'] = array(
    '#type' => 'select',
    '#title' => t('Payment mode'),
    '#options' => array(
      'test' => t('Test mode'),
      'live' => t('Live mode')
    ),
    '#default_value' => variable_get('money_mode', 'test'),
  );

  $form['money_all']['money_payment_method'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled payment methods'),
    '#options' => money_get_payment_methods(),
    '#default_value' => variable_get('money_payment_method', array_keys(money_get_payment_methods())),
  );

  $form['money_all']['money_default_payment_method'] = array(
    '#type' => 'radios',
    '#title' => t('Default payment method'),
    '#options' => money_get_payment_methods(),
    '#default_value' => variable_get('money_default_payment_method', money_DEFAULT_PAYMENT_METHOD),
  );

  // Shop settings
  $shop_states = array(
    // Hide the settings when the Yandex shop checkbox is disabled.
    'invisible' => array(
      ':input[name="money_shop"]' => array('checked' => FALSE),
    ),
  );
  $form['money_shop_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Yandex shop settings'),
  );
  $form['money_shop_setting']['money_shop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Select if you have a shop in Yandex.Money'),
    '#default_value' => variable_get('money_shop', 0),
  );
  $form['money_shop_setting']['money_shop_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Shop ID'),
    '#description' => t('You shop ID. If you have any shops in you Yandex account.'),
    '#default_value' => variable_get('money_shop_id', ''),
    '#size' => 2,
    '#states' => $shop_states,
  );
  $form['money_shop_setting']['money_scid'] = array(
    '#type' => 'textfield',
    '#title' => t('Shop SCID'),
    '#default_value' => variable_get('money_scid', ''),
    '#size' => 5,
    '#states' => $shop_states,
  );
  $form['money_shop_setting']['money_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Shop security key'),
    '#description' => t('You shop password security key. Not you payment password.'),
    '#default_value' => variable_get('money_secret', ''),
    '#size' => 17,
    '#states' => $shop_states,
  );

  // Purse settings
  $form['money_purse_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Yandex purse settings'),
  );
  $form['money_purse_setting']['money_receiver'] = array(
    '#type' => 'textfield',
    '#title' => t('Purse number'),
    '#description' => t('Your Yandex.Money purse number.'),
    '#default_value' => variable_get('money_receiver', ''),
    '#size' => 14,
  );

  $form['money_purse_setting']['money_formcomment'] = array(
    '#type' => 'textfield',
    '#title' => t('Pay comment'),
    '#description' => t('Your Yandex.Money pay comment.'),
    '#default_value' => variable_get('money_formcomment', ''),
  );

  $form['money_texts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Text for success and fail payment pages'),
  );
  $success = variable_get('money_success_text', '');
  $form['money_texts']['money_success_text'] = array(
    '#type' => 'text_format',
    '#title' => t('Text for success page'),
    '#default_value' => $success['value'] ? $success['value'] : '',
    '#format' => $success['format'] ? $success['format'] : '',
  );
  $fail = variable_get('money_fail_text', '');
  $form['money_texts']['money_fail_text'] = array(
    '#type' => 'text_format',
    '#title' => t('Text for fail page'),
    '#default_value' => $fail['value'] ? $fail['value'] : '',
    '#format' => $fail['format'] ? $fail['format'] : '',
  );
  return system_settings_form($form);
}

/**
 * @param array $form
 * @param array $form_state
 */
function money_admin_settings_validate($form, &$form_state) {
  if (!$form_state['values']['money_payment_method']) {
    form_set_error('money_payment_method', t('You do not choose the type of payment online!'));
  }

  if ($form_state['values']['money_ip']) {
    $ips = explode("\n", $form_state['values']['money_ip']);
    foreach ($ips as $ip) {
      $ip = trim($ip);
      if (empty($ip)) {
        continue;
      }
      if (!filter_var($ip, FILTER_VALIDATE_IP)) {
        form_set_error('money_ip', t('IP is not correct: %ip.<br/>Please insert correct server IP or insert 0.0.0.0 to default!', array(
          '%ip' => $ip,
        )));
      }
    }
  }
}