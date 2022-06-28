/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

export type ProviderPayoutStatus = 'off' | 'processing' | 'complete'

export function getProviderPayoutStatus (
  payoutStatus: any, walletProvider: string
): ProviderPayoutStatus {
  if (!payoutStatus) {
    return 'off'
  }

  if (walletProvider && payoutStatus[walletProvider]) {
    return payoutStatus[walletProvider]
  }

  return payoutStatus.unverified
}
