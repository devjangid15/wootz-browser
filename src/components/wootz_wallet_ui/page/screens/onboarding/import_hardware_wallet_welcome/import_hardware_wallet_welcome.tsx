// Copyright (c) 2024 The Wootz Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { useHistory } from 'react-router'

// utils
import { getLocale, splitStringForTag } from '../../../../../common/locale'
import { WalletRoutes } from '../../../../constants/types'

// components
import {
  OnboardingContentLayout //
} from '../components/onboarding_content_layout/content_layout'

// styles
import {
  Bold,
  Description,
  HardwareGraphic
} from './import_hardware_wallet_welcome.style'
import { ContinueButton } from '../onboarding.style'
import { Column, VerticalSpace } from '../../../../components/shared/style'

export const OnboardingImportHardwareWalletWelcome = () => {
  const history = useHistory()

  const onClickContinue = () => {
    history.push(WalletRoutes.OnboardingHardwareWalletCreatePassword)
  }

  const { beforeTag, duringTag, afterTag } = splitStringForTag(
    getLocale('wootzWallectConnectHardwareDescription')
  )

  return (
    <OnboardingContentLayout
      title={getLocale('wootzWalletConnectHardwareWallet')}
    >
      <HardwareGraphic />
      <Description>
        {getLocale('wootzWalletImportHardwareWalletDescription')}
      </Description>
      <VerticalSpace space='28px' />
      <Description>
        {beforeTag}
        <Bold>{getLocale('wootzWalletConnectHardwareLedger')}</Bold>
        {duringTag}
        <Bold>{getLocale('wootzWalletConnectHardwareTrezor')}</Bold>
        {afterTag}
      </Description>
      <Column margin='117px 0 88px 0'>
        <ContinueButton onClick={onClickContinue}>
          {getLocale('wootzWalletButtonContinue')}
        </ContinueButton>
      </Column>
    </OnboardingContentLayout>
  )
}
