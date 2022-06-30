// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { useSelector } from 'react-redux'

// types
import {
  BraveWallet,
  BuyOption,
  BuySendSwapViewTypes,
  ToOrFromType,
  WalletState
} from '../../../constants/types'

// utils
import { getLocale } from '../../../../common/locale'
import { getRampAssetSymbol, isSelectedAssetInAssetOptions } from '../../../utils/asset-utils'

// options
import { BuyOptions } from '../../../options/buy-with-options'
import { SelectBuyOption } from '../select-buy-option/select-buy-option'

// hooks
import { useAssets } from '../../../common/hooks/assets'
import { useLib } from '../../../common/hooks/useLib'

// components
import { NavButton } from '../../extension'
import SwapInputComponent from '../swap-input-component'

// styles
import {
  StyledWrapper,
  Spacer
} from './style'

export interface Props {
  selectedAsset: BraveWallet.BlockchainToken
  onChangeBuyView: (view: BuySendSwapViewTypes, option?: ToOrFromType) => void
}

export const Buy = ({
  selectedAsset,
  onChangeBuyView
}: Props) => {
  // state
  const [buyAmount, setBuyAmount] = React.useState('')
  const [showBuyOptions, setShowBuyOptions] = React.useState<boolean>(false)

  // Redux
  const {
    selectedNetwork,
    selectedAccount,
    defaultCurrencies
  } = useSelector(({ wallet }: { wallet: WalletState }) => wallet)

  // Custom Hooks
  const { wyreAssetOptions, rampAssetOptions } = useAssets()
  const { getBuyAssetUrl } = useLib()

  // memos
  const supportingBuyOptions: BuyOption[] = React.useMemo(() => {
    return BuyOptions.filter(buyOption => {
      if (buyOption.id === BraveWallet.OnRampProvider.kWyre) {
        return isSelectedAssetInAssetOptions(selectedAsset, wyreAssetOptions)
      }

      if (buyOption.id === BraveWallet.OnRampProvider.kRamp) {
        return isSelectedAssetInAssetOptions(selectedAsset, rampAssetOptions)
      }

      return false
    })
  }, [selectedAsset, wyreAssetOptions, rampAssetOptions])

  const isSelectedNetworkSupported: boolean = React.useMemo(() => {
    return [...rampAssetOptions, ...wyreAssetOptions]
    .map(asset => asset.chainId.toLowerCase())
    .includes(selectedNetwork.chainId.toLowerCase())
  }, [rampAssetOptions, wyreAssetOptions, selectedNetwork])

  // methods
  const onSubmitBuy = React.useCallback((buyOption: BraveWallet.OnRampProvider) => {
    const asset = buyOption === BraveWallet.OnRampProvider.kRamp
      ? { ...selectedAsset, symbol: getRampAssetSymbol(selectedAsset) }
      : selectedAsset

    getBuyAssetUrl({
      asset,
      onRampProvider: buyOption,
      chainId: selectedNetwork.chainId,
      address: selectedAccount.address,
      amount: buyAmount
    })
      .then(url => {
        chrome.tabs.create({ url }, () => {
          if (chrome.runtime.lastError) {
            console.error('tabs.create failed: ' + chrome.runtime.lastError.message)
          }
        })
      })
      .catch(e => console.error(e))
  }, [selectedAsset, getBuyAssetUrl, selectedNetwork, selectedAccount, buyAmount])

  const onShowAssets = React.useCallback(() => {
    onChangeBuyView('assets', 'from')
  }, [onChangeBuyView])

  const onContinue = React.useCallback(() => {
    setShowBuyOptions(true)
  }, [])

  const onBack = React.useCallback(() => {
    setShowBuyOptions(false)
  }, [])

  // render
  return (
    <StyledWrapper>
      {showBuyOptions
        ? <SelectBuyOption
          buyOptions={supportingBuyOptions}
          onSelect={onSubmitBuy}
          onBack={onBack}
        />
        : <>
          {isSelectedNetworkSupported &&
            <SwapInputComponent
              defaultCurrencies={defaultCurrencies}
              componentType='buyAmount'
              onInputChange={setBuyAmount}
              selectedAssetInputAmount={buyAmount}
              inputName='buy'
              selectedAsset={selectedAsset}
              selectedNetwork={selectedNetwork}
              onShowSelection={onShowAssets}
              autoFocus={true}
            />
          }
          <Spacer />
          <NavButton
            disabled={false}
            buttonType='primary'
            text={getLocale('braveWalletBuyContinueButton')}
            onSubmit={onContinue}
          />
        </>
      }
    </StyledWrapper>
  )
}

export default Buy
