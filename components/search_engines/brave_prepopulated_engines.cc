/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/search_engines/brave_prepopulated_engines.h"
#include "components/search_engines/prepopulated_engines.h"
#include "components/search_engines/search_engine_type.h"

namespace TemplateURLPrepopulateData {

// IMPORTANT! Make sure to bump this value if you make changes to the
// engines below or add/remove engines.
const int kBraveCurrentDataVersion = 1;

const PrepopulatedEngine amazon = {
  L"Amazon",
  L":a",
  "https://www.amazon.com/favicon.ico",
  "https://www.amazon.com/exec/obidos/external-search/?field-keywords={searchTerms}&mode=blended",
  "UTF-8",
  "https://completion.amazon.com/search/complete?method=completion&q={searchTerms}&search-alias=aps&client=amazon-search-ui&mkt=1",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_AMAZON,
};

const PrepopulatedEngine duckduckgo = {
  L"DuckDuckGo",
  L":d",
  "https://duckduckgo.com/favicon.ico",
  "https://duckduckgo.com/?q={searchTerms}&t=brave",
  "UTF-8",
  "https://ac.duckduckgo.com/ac/?q={searchTerms}&type=list",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
};

const PrepopulatedEngine ecosia = {
  L"Ecosia",
  L":e",
  "https://cdn.ecosia.org/assets/images/ico/favicon.ico",
  "https://www.ecosia.org/search?q={searchTerms}",
  "UTF-8",
  "https://ac.ecosia.org/autocomplete?q={searchTerms}&type=list",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_ECOSIA,
};

const PrepopulatedEngine findx = {
  L"findx",
  L":fx",
  "https://www.findx.com/favicon.ico",
  "https://www.findx.com/search?q={searchTerms}&type=web",
  "UTF-8",
  "https://www.findx.com/api/web-search/suggestions/?q={searchTerms}&type=opensearch",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_FINDX,
};

const PrepopulatedEngine github = {
  L"GitHub",
  L":gh",
  "https://www.github.com/favicon.ico",
  "https://github.com/search?q={searchTerms}",
  "UTF-8",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_GITHUB,
};

const PrepopulatedEngine infogalactic = {
  L"Infogalactic",
  L":i",
  "https://www.infogalactic.com/favicon.ico",
  "https://infogalactic.com/w/index.php?title=Special:Search&search={searchTerms}",
  "UTF-8",
  "https://infogalactic.com/w/api.php?action=opensearch&search={searchTerms}&namespace=0",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_INFOGALACTIC,
};

const PrepopulatedEngine mdnwebdocs = {
  L"MDN Web Docs",
  L":m",
  "https://developer.mozilla.org/favicon.ico",
  "https://developer.mozilla.org/search?q={searchTerms}",
  "UTF-8",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_MDNWEBDOCS,
};

const PrepopulatedEngine qwant = {
  L"Qwant",
  L":q",
  "https://www.qwant.com/favicon.ico",
  "https://www.qwant.com/?q={searchTerms}&client=brz-brave",
  "UTF-8",
  "https://api.qwant.com/api/suggest/?q={searchTerms}&client=opensearch",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_QWANT,
};

const PrepopulatedEngine searx = {
  L"searx",
  L":x",
  "https://searx.me/favicon.ico",
  "https://searx.me/?q={searchTerms}&categories=general",
  "UTF-8",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_SEARX,
};

const PrepopulatedEngine semanticscholar = {
  L"Semantic Scholar",
  L":ss",
  "https://www.semanticscholar.org/img/favicon.png",
  "https://www.semanticscholar.org/search?q={searchTerms}",
  "UTF-8",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_SEMANTICSCHOLAR,
};

const PrepopulatedEngine stackoverflow = {
  L"Stack Overflow",
  L":s",
  "https://cdn.sstatic.net/Sites/stackoverflow/img/favicon.ico",
  "https://stackoverflow.com/search?q={searchTerms}",
  "UTF-8",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_STACKOVERFLOW,
};

const PrepopulatedEngine startpage = {
  L"StartPage",
  L":sp",
  "https://www.startpage.com/favicon.ico",
  "https://www.startpage.com/do/dsearch?query={searchTerms}&cat=web&pl=opensearch",
  "UTF-8",
  "https://www.startpage.com/cgi-bin/csuggest?query={searchTerms}&limit=10&format=json",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_STARTPAGE,
};

const PrepopulatedEngine twitter = {
  L"Twitter",
  L":t",
  "https://twitter.com/favicon.ico",
  "https://twitter.com/search?q={searchTerms}&source=desktop-search",
  "UTF-8",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_TWITTER,
};

const PrepopulatedEngine wikipedia = {
  L"Wikipedia",
  L":w",
  "https://en.wikipedia.org/favicon.ico",
  "https://en.wikipedia.org/wiki/Special:Search?search={searchTerms}",
  "UTF-8",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_WIKIPEDIA,
};

const PrepopulatedEngine wolframalpha = {
  L"Wolfram Alpha",
  L":wa",
  "https://www.wolframalpha.com/favicon.ico",
  "https://www.wolframalpha.com/input/?i={searchTerms}",
  "UTF-8",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_WOLFRAMALPHA,
};

const PrepopulatedEngine yandex = {
  L"Yandex",
  L":ya",
  "https://yandex.st/lego/_/rBTjd6UOPk5913OSn5ZQVYMTQWQ.ico",
  "https://yandex.com/search/?text={searchTerms}&clid=2274777",
  "UTF-8",
  "https://suggest.yandex.com/suggest-ff.cgi?part={searchTerms}",
  "https://yandex.com/images/search/?rpt=imageview",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  "upfile={google:imageThumbnail},original_width={google:imageOriginalWidth},original_height={google:imageOriginalHeight},prg=1",
  NULL,
  0,
  SEARCH_ENGINE_YANDEX,
  15,
};

const PrepopulatedEngine youtube = {
  L"YouTube",
  L":yt",
  "https://www.youtube.com/favicon.ico",
  "https://www.youtube.com/results?search_type=search_videos&search_query={searchTerms}&search_sort=relevance&search_category=0&page=",
  "UTF-8",
  "https://suggestqueries.google.com/complete/search?output=chrome&client=chrome&hl=it&q={searchTerms}&ds=yt",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  0,
  SEARCH_ENGINE_OTHER,
  PREPOPULATED_ENGINE_ID_YOUTUBE,
};

}  // namespace TemplateURLPrepopulateData
