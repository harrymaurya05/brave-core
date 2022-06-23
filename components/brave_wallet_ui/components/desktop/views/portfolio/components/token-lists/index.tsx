// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { useHistory } from 'react-router'

// Types
import {
  BraveWallet,
  UserAssetInfoType,
  WalletRoutes
} from '../../../../../../constants/types'

// Utils
import { getLocale } from '../../../../../../../common/locale'

// Components
import { SearchBar } from '../../../../../shared'
import {
  AddButton,
  NetworkFilterSelector
} from '../../../../'

// Styled Components
import { ScrollableColumn } from '../../../../../shared/style'
import {
  ButtonRow,
  DividerText,
  SubDivider,
  Spacer,
  FilterTokenRow
} from '../../style'

export interface Props {
  userAssetList: UserAssetInfoType[]
  networks: BraveWallet.NetworkInfo[]
  renderToken: (item: UserAssetInfoType) => JSX.Element
  hideAddButton?: boolean
  enableScroll?: boolean
  maxListHeight?: string
}

const TokenLists = ({
  userAssetList,
  networks,
  renderToken,
  hideAddButton,
  enableScroll,
  maxListHeight
}: Props) => {
  // routing
  const history = useHistory()

  // state
  const [searchText, setSearchText] = React.useState<string>('')

  // memos
  const filteredAssetList = React.useMemo(() => {
    if (searchText === '') {
      return userAssetList
    }
    return userAssetList.filter((item) => {
      return (
        item.asset.name.toLowerCase() === searchText.toLowerCase() ||
        item.asset.name.toLowerCase().startsWith(searchText.toLowerCase()) ||
        item.asset.symbol.toLocaleLowerCase() === searchText.toLowerCase() ||
        item.asset.symbol.toLowerCase().startsWith(searchText.toLowerCase())
      )
    })
  }, [searchText, userAssetList])

  const nonFungibleTokenList = React.useMemo(
    () => filteredAssetList.filter(({ asset }) => asset.isErc721).map(renderToken),
    [filteredAssetList, renderToken]
  )

  const fungibleTokenList = React.useMemo(
    () => filteredAssetList.filter(({ asset }) => !asset.isErc721).map(renderToken),
    [filteredAssetList, renderToken]
  )

  // methods
  const onSearchText = React.useCallback((event: React.ChangeEvent<HTMLInputElement>) => {
    setSearchText(event.target.value)
  }, [])

  const showAddAssetsModal = React.useCallback(() => {
    history.push(WalletRoutes.AddAssetModal)
  }, [])

  // effects
  React.useEffect(() => {
    if (userAssetList) {
      setSearchText('')
    }
  }, [userAssetList])

  // render
  return (
    <>
      <FilterTokenRow>
        <SearchBar
          placeholder={getLocale('braveWalletSearchText')}
          action={onSearchText}
          value={searchText}
        />
        <NetworkFilterSelector networkListSubset={networks} />
      </FilterTokenRow>

      <ScrollableColumn scrollDisabled={!enableScroll} maxHeight={maxListHeight}>
        {fungibleTokenList}

        {nonFungibleTokenList.length !== 0 &&
          <>
            <Spacer />
            <DividerText>{getLocale('braveWalletTopNavNFTS')}</DividerText>
            <SubDivider />
            {nonFungibleTokenList}
          </>
        }
      </ScrollableColumn>

      {!hideAddButton && <ButtonRow>
        <AddButton
          buttonType='secondary'
          onSubmit={showAddAssetsModal}
          text={getLocale('braveWalletAccountsEditVisibleAssets')}
        />
      </ButtonRow>}
    </>
  )
}

export default TokenLists
