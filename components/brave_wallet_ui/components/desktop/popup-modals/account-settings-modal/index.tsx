// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'

// utils
import { getLocale, getLocaleWithTag } from '../../../../../common/locale'
import { generateQRCode } from '../../../../utils/qr-code-utils'
import { reduceAddress } from '../../../../utils/reduce-address'
import { copyToClipboard } from '../../../../utils/copy-to-clipboard'

// options
import {
  AccountSettingsNavOptions,
  HardwareAccountSettingsNavOptions
} from '../../../../options/account-settings-nav-options'

// types
import {
  AccountSettingsNavTypes,
  BraveWallet,
  WalletAccountType,
  UpdateAccountNamePayloadType,
  TopTabNavObjectType
} from '../../../../constants/types'

// hooks
import { useIsMounted } from '../../../../common/hooks/useIsMounted'

// components
import {
  PopupModal,
  TopTabNav
} from '../..'
import { FILECOIN_FORMAT_DESCRIPTION_URL } from '../../../../common/constants/urls'
import { NavButton } from '../../../extension'
import { Tooltip } from '../../../shared'

// Styled Components
import {
  Input,
  StyledWrapper,
  QRCodeWrapper,
  AddressButton,
  ButtonRow,
  CopyIcon,
  PrivateKeyWrapper,
  WarningText,
  WarningWrapper,
  PrivateKeyBubble,
  ButtonWrapper,
  ErrorText
} from './style'

export interface Props {
  onClose: () => void
  onUpdateAccountName: (payload: UpdateAccountNamePayloadType) => { success: boolean }
  onCopyToClipboard: () => void
  onChangeTab: (id: AccountSettingsNavTypes) => void
  onRemoveAccount: (address: string, hardware: boolean, coin: BraveWallet.CoinType) => void
  onViewPrivateKey: (address: string, isDefault: boolean, coin: BraveWallet.CoinType) => void
  onDoneViewingPrivateKey: () => void
  onToggleNav: () => void
  privateKey: string
  hideNav: boolean
  tab: AccountSettingsNavTypes
  title: string
  account: WalletAccountType
}

const AddAccountModal = (props: Props) => {
  const {
    title,
    account,
    tab,
    hideNav,
    privateKey,
    onClose,
    onToggleNav,
    onUpdateAccountName,
    onCopyToClipboard,
    onChangeTab,
    onRemoveAccount,
    onViewPrivateKey,
    onDoneViewingPrivateKey
  } = props

  // custom hooks
  const isMounted = useIsMounted()

  // state
  const [accountName, setAccountName] = React.useState<string>(account.name)
  const [showPrivateKey, setShowPrivateKey] = React.useState<boolean>(false)
  const [updateError, setUpdateError] = React.useState<boolean>(false)
  const [qrCode, setQRCode] = React.useState<string>('')

  // methods
  const handleAccountNameChanged = (event: React.ChangeEvent<HTMLInputElement>) => {
    setAccountName(event.target.value)
    setUpdateError(false)
  }

  const onSubmitUpdateName = () => {
    const isDerived = account?.accountType === 'Primary'
    const payload = {
      address: account.address,
      name: accountName,
      isDerived: isDerived
    }
    onUpdateAccountName(payload).success ? onClose() : setUpdateError(true)
  }

  const generateQRData = React.useCallback(() => {
    generateQRCode(account.address).then(qr => {
      if (isMounted) {
        setQRCode(qr)
      }
    })
  }, [account, isMounted])

  React.useEffect(() => {
    generateQRData()
  })

  const onSelectTab = (id: AccountSettingsNavTypes) => {
    onChangeTab(id)
  }

  const removeAccount = () => {
    onRemoveAccount(account.address, false, account.coin)
    onToggleNav()
    onClose()
  }

  const onShowPrivateKey = () => {
    if (onViewPrivateKey) {
      const isDefault = account?.accountType === 'Primary'
      onViewPrivateKey(account?.address ?? '', isDefault, account?.coin)
    }
    setShowPrivateKey(true)
  }

  const onHidePrivateKey = () => {
    onDoneViewingPrivateKey()
    setShowPrivateKey(false)
  }

  const onClickClose = () => {
    onHidePrivateKey()
    setUpdateError(false)
    onClose()
  }

  const onCopyPrivateKey = async () => {
    if (privateKey) {
      await copyToClipboard(privateKey)
    }
  }

  const handleKeyDown = (event: React.KeyboardEvent<HTMLInputElement>) => {
    if (event.key === 'Enter' && accountName) {
      onSubmitUpdateName()
    }
  }

  const tabList = React.useMemo((): TopTabNavObjectType[] => {
    return account.accountType === 'Trezor' ||
      account.accountType === 'Ledger'
      ? HardwareAccountSettingsNavOptions()
      : AccountSettingsNavOptions()
  }, [account])

  const filPrivateKeyFormatDescriptionTextParts =
      getLocaleWithTag('braveWalletFilExportPrivateKeyFormatDescription')

  return (
    <PopupModal title={title} onClose={onClickClose}>
      {!hideNav &&
        <TopTabNav
          tabList={tabList}
          onSelectTab={onSelectTab}
          selectedTab={tab}
        />
      }
      <StyledWrapper>
        {tab === 'details' &&
          <>
            <Input
              value={accountName}
              placeholder={getLocale('braveWalletAddAccountPlaceholder')}
              onChange={handleAccountNameChanged}
              onKeyDown={handleKeyDown}
            />
            {updateError &&
              <ErrorText>{getLocale('braveWalletAccountSettingsUpdateError')}</ErrorText>
            }
            <QRCodeWrapper src={qrCode} />
            <Tooltip text={getLocale('braveWalletToolTipCopyToClipboard')}>
              <AddressButton onClick={onCopyToClipboard}>{reduceAddress(account.address)}<CopyIcon /></AddressButton>
            </Tooltip>
            <ButtonRow>
              <NavButton
                onSubmit={onSubmitUpdateName}
                disabled={!accountName}
                text={getLocale('braveWalletAccountSettingsSave')}
                buttonType='secondary'
              />
              {account?.accountType === 'Secondary' &&
                <NavButton
                  onSubmit={removeAccount}
                  text={getLocale('braveWalletAccountSettingsRemove')}
                  buttonType='danger'
                />
              }
            </ButtonRow>
          </>
        }
        {tab === 'privateKey' &&
          <PrivateKeyWrapper>
            <WarningWrapper>
              <WarningText>{getLocale('braveWalletAccountSettingsDisclaimer')}</WarningText>
            </WarningWrapper>
            {showPrivateKey && account.coin === BraveWallet.CoinType.FIL &&
            <WarningWrapper>
               <WarningText>
                 {filPrivateKeyFormatDescriptionTextParts.beforeTag}
                  <a target='_blank' href={FILECOIN_FORMAT_DESCRIPTION_URL}>
                    {filPrivateKeyFormatDescriptionTextParts.duringTag}
                  </a>
                  {filPrivateKeyFormatDescriptionTextParts.afterTag}
                </WarningText>
            </WarningWrapper>
            }
            {showPrivateKey &&
              <Tooltip text={getLocale('braveWalletToolTipCopyToClipboard')}>
                <PrivateKeyBubble onClick={onCopyPrivateKey}>{privateKey}</PrivateKeyBubble>
              </Tooltip>
            }
            <ButtonWrapper>
              <NavButton
                onSubmit={!showPrivateKey ? onShowPrivateKey : onHidePrivateKey}
                text={!showPrivateKey ? getLocale('braveWalletAccountSettingsShowKey') : getLocale('braveWalletAccountSettingsHideKey')}
                buttonType='primary'
              />
            </ButtonWrapper>
          </PrivateKeyWrapper>
        }
      </StyledWrapper>
    </PopupModal>
  )
}

export default AddAccountModal
