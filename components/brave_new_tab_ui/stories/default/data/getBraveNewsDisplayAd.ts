// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import { DisplayAd } from '../../../api/brave_news/brave_news_proxy'

let callCount = 0

export default function getBraveNewsDisplayAd (always: boolean = false) {
  callCount++
  const ad: DisplayAd | null = (always || callCount % 2)
    ? {
      description: 'Technica',
      uuid: '0abc',
      creative_instance_id: '1234',
      title: '10 reasons why technica recreated the sound of old classics.',
      cta_text: 'Hear it',
      target_url: { url: 'https://www.brave.com' },
      image: { padded_image_url: { url: 'https://pcdn.brave.software/brave-today/cache/0e9f8fa60d995c1ca86f924b6104195c40555f696598b2f772b27d9b954ce158.jpg.pad' } },
      dimensions: '1x3'
    }
    : null
  return new Promise<DisplayAd | null>(function (resolve) {
    setTimeout(() => resolve(ad), 2400)
  })
}
