/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as BraveWallet from 'gen/brave/components/brave_wallet/common/brave_wallet.mojom.m.js'
import { getRampNetworkPrefix, getWyreNetworkPrefix } from './string-utils'

export const getUniqueAssets = (assets: BraveWallet.BlockchainToken[]) => {
  return assets.filter((asset, index) => {
    return index === assets.findIndex(item => {
      return item.contractAddress.toLowerCase() === asset.contractAddress.toLowerCase() && item.chainId === asset.chainId
    })
  })
}

export const isSelectedAssetInAssetOptions = (
  selectedAsset: BraveWallet.BlockchainToken,
  assetOptions: BraveWallet.BlockchainToken[]
) => {
  return assetOptions.findIndex(asset => {
    return asset.contractAddress.toLowerCase() === selectedAsset?.contractAddress.toLowerCase() &&
      asset.chainId === selectedAsset.chainId &&
      asset.symbol.toLowerCase() === selectedAsset.symbol.toLowerCase()
  }) !== -1
}

export const getWyreAssetSymbol = (asset: BraveWallet.BlockchainToken) => {
  if (
    !asset.contractAddress || // gas coins ok
    asset.chainId === BraveWallet.MAINNET_CHAIN_ID // ETH-ERC coins ok
  ) {
    return asset.symbol
  }

  // format non-ethereum EVM token symbols for Wyre
  const prefix = getWyreNetworkPrefix(asset.chainId)
  return prefix ? `${prefix}${asset.symbol.toUpperCase()}` : asset.symbol
}

export const getRampAssetSymbol = (asset: BraveWallet.BlockchainToken) => {
  if (asset.symbol.toUpperCase() === 'BAT' && asset.chainId === BraveWallet.MAINNET_CHAIN_ID) {
    // BAT is the only token on Ethereum Mainnet with a prefix on Ramp.Network
    return 'ETH_BAT'
  }

  if (asset.chainId === BraveWallet.AVALANCHE_MAINNET_CHAIN_ID && asset.contractAddress === '') {
    return asset.symbol // AVAX native token has no prefix
  }

  const rampNetworkPrefix = getRampNetworkPrefix(asset.chainId)
  return rampNetworkPrefix !== '' ? `${rampNetworkPrefix}_${asset.symbol.toUpperCase()}` : asset.symbol
}

export const addLogoToToken = (token: BraveWallet.BlockchainToken) => {
  return {
    ...token,
    logo: `chrome://erc-token-images/${token.logo}`
  }
}
