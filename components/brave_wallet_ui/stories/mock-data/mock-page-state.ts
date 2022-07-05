import { BraveWallet, PageState } from '../../constants/types'
import { mockedMnemonic } from './user-accounts'

export const mockPageState: PageState = {
  hasInitialized: false,
  importAccountError: false,
  importWalletError: {
    hasError: false
  },
  invalidMnemonic: false,
  isCryptoWalletsInitialized: false,
  isFetchingPriceHistory: false,
  isMetaMaskInitialized: false,
  portfolioPriceHistory: [],
  selectedAsset: undefined,
  selectedAssetCryptoPrice: undefined,
  selectedAssetFiatPrice: undefined,
  selectedAssetPriceHistory: [],
  selectedTimeline: BraveWallet.AssetPriceTimeframe.OneDay,
  setupStillInProgress: false,
  showAddModal: false,
  showIsRestoring: false,
  showRecoveryPhrase: false,
  isImportWalletsCheckComplete: false,
  importWalletAttempts: 0,
  walletTermsAcknowledged: false,
  mnemonic: mockedMnemonic
    .replace('tomato', 'FIRST')
    .replace('velvet', 'THIRD')
    .concat(` ${mockedMnemonic} LAST`) // 25 word phrase
}
