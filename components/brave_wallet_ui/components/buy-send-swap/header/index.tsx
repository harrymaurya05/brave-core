// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { useSelector } from 'react-redux'
import { create } from 'ethereum-blockies'

// types
import { BuySendSwapViewTypes, WalletState } from '../../../constants/types'

// utils
import { getLocale } from '../../../../common/locale'
import { reduceAddress } from '../../../utils/reduce-address'
import { reduceAccountDisplayName } from '../../../utils/reduce-account-name'

// hooks
import { useCopyToClipboard } from '../../../common/hooks/use-copy-to-clipboard'

// components
import { Tooltip, SelectNetworkButton } from '../../shared'

// Styled Components
import {
  StyledWrapper,
  AccountAddress,
  AccountAndAddress,
  AccountCircle,
  AccountName,
  NameAndIcon,
  SwitchIcon
} from './style'

export interface Props {
  onChangeSwapView: (view: BuySendSwapViewTypes) => void
}

export const SwapHeader = ({ onChangeSwapView }: Props) => {
  // redux
  const selectedAccount = useSelector(({ wallet }: {wallet: WalletState}) => wallet.selectedAccount)
  const selectedNetwork = useSelector(({ wallet }: {wallet: WalletState}) => wallet.selectedNetwork)

  // hooks
  const {
    isCopied: copied,
    copyToClipboard: copyText
  } = useCopyToClipboard(1500)

  // methods
  const onShowAccounts = React.useCallback(() => {
    onChangeSwapView('acounts')
  }, [onChangeSwapView])

  const onShowNetworks = React.useCallback(() => {
    onChangeSwapView('networks')
  }, [onChangeSwapView])

  const onCopyToClipboard = React.useCallback(async () => {
    await copyText(selectedAccount.address)
  }, [copyText, selectedAccount])

  // memos
  const orb = React.useMemo(() => {
    return create({ seed: selectedAccount.address.toLowerCase(), size: 8, scale: 16 }).toDataURL()
  }, [selectedAccount])

  // render
  return (
    <StyledWrapper>
      <NameAndIcon>
        <AccountCircle onClick={onShowAccounts} orb={orb}>
          <SwitchIcon />
        </AccountCircle>
        <Tooltip
          text={getLocale('braveWalletToolTipCopyToClipboard')}
          actionText={getLocale('braveWalletToolTipCopiedToClipboard')}
          isActionVisible={copied}
        >
          <AccountAndAddress onClick={onCopyToClipboard}>
            <AccountName>{reduceAccountDisplayName(selectedAccount.name, 11)}</AccountName>
            <AccountAddress>{reduceAddress(selectedAccount.address)}</AccountAddress>
          </AccountAndAddress>
        </Tooltip>
      </NameAndIcon>
      <Tooltip text={selectedNetwork.chainName}>
        <SelectNetworkButton
          selectedNetwork={selectedNetwork}
          onClick={onShowNetworks}
        />
      </Tooltip>
    </StyledWrapper >
  )
}

export default SwapHeader
