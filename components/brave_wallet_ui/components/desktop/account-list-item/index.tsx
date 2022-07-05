// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { create } from 'ethereum-blockies'

// utils
import { getLocale } from '../../../../common/locale'
import { reduceAddress } from '../../../utils/reduce-address'

// types
import {
  BraveWallet,
  WalletAccountType
} from '../../../constants/types'

// hooks
import { useCopyToClipboard } from '../../../common/hooks/use-copy-to-clipboard'

// components
import { Tooltip } from '../../shared/tooltip/index'

// style
import {
  StyledWrapper,
  AccountName,
  AccountAddress,
  AccountAndAddress,
  NameAndIcon,
  AccountCircle,
  RightSide,
  HardwareIcon,
  AccountNameRow,
  DeleteButton,
  DeleteIcon
} from './style'

export interface Props {
  onDelete?: () => void
  onClick: (account: WalletAccountType) => void
  account: WalletAccountType
  isHardwareWallet: boolean
  onRemoveAccount: (address: string, hardware: boolean, coin: BraveWallet.CoinType) => void
}

export const AccountListItem = ({
  account,
  isHardwareWallet,
  onClick,
  onRemoveAccount
}: Props) => {
  // custom hooks
  const {
    isCopied: copied,
    copyToClipboard: copyText
  } = useCopyToClipboard(1500)

  // methods
  const onCopyToClipboard = React.useCallback(async () => {
    await copyText(account.address)
  }, [copyText, account.address])

  const onSelectAccount = React.useCallback(() => {
    onClick(account)
  }, [onClick, account])

  const removeAccount = React.useCallback(() => {
    let confirmAction = confirm(`Are you sure to remove ${account.name}?`)
    if (confirmAction) {
      onRemoveAccount(account.address, isHardwareWallet, account.coin)
    }
  }, [account, isHardwareWallet, onRemoveAccount])

  // memos
  const orb = React.useMemo(() => {
    return create({ seed: account.address.toLowerCase(), size: 8, scale: 16 }).toDataURL()
  }, [account.address])

  // render
  return (
    <StyledWrapper>
      <NameAndIcon>
        <AccountCircle orb={orb} />
        <AccountAndAddress>
          <AccountNameRow>
            {isHardwareWallet && <HardwareIcon />}
            <AccountName onClick={onSelectAccount}>{account.name}</AccountName>
          </AccountNameRow>
          <Tooltip
            text={getLocale('braveWalletToolTipCopyToClipboard')}
            actionText={getLocale('braveWalletToolTipCopiedToClipboard')}
            isActionVisible={copied}
          >
            <AccountAddress onClick={onCopyToClipboard}>{reduceAddress(account.address)}</AccountAddress>
          </Tooltip>
        </AccountAndAddress>
      </NameAndIcon>
      <RightSide>
        {(account.accountType !== 'Primary') &&
          <DeleteButton onClick={removeAccount}>
            <DeleteIcon />
          </DeleteButton>
        }
      </RightSide>
    </StyledWrapper>
  )
}

export default AccountListItem
