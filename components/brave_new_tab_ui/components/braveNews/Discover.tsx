import TextInput from '$web-components/input'
import * as React from 'react'
import styled from 'styled-components'
import Flex from '../Flex'
import Button from '$web-components/button'
import CategoryCard from './CategoryCard'
import DiscoverSection from './DiscoverSection'
import { DoubleHeart, Rocket, History } from './Icons'
import SourceCard from './SourceCard'
import { useCategories } from '../../hooks/braveNews'

const Header = styled.span`
    font-size: 24px;
    font-weight: 600;
    padding: 12px 0;
`

const SearchInput = styled(TextInput)`
    margin: 16px 0;
    border-radius: 4px;
    --interactive8: #AEB1C2;
    --focus-border: #737ADE;
`

const Link = styled.a`
    color: #4C54D2;
    font-weight: 600;
`
const SuggestedSubtitle = <>
    Some sources you might like. Suggestions are anonymous, and matched only on your device.&nbsp;
    <Link>Learn more</Link>
</>

const LoadMoreButton = styled(Button)`
    grid-column: 2;
`

// The default number of category cards to show.
const DEFAULT_NUM_CATEGORIES = 3;
export default function Discover(props: {}) {
    const categories = useCategories();
    const [showingAllCategories, setShowingAllCategories] = React.useState(false);

    return <Flex direction='column'>
        <Header>Discover</Header>
        <SearchInput type="search" placeholder='Search for news, site, topic or RSS feed' />
        <DiscoverSection name='Trending' sectionId='trending' >
            <SourceCard icon={Rocket} text="Popular" id='popular' background='#353DAB' />
            <SourceCard icon={History} text="Newly added" id='new' background='#207DC9' />
            <SourceCard icon={DoubleHeart} text="Suggested" id='suggested' background='#FB542B' />
        </DiscoverSection>
        <DiscoverSection name='Browse by category' sectionId='categories'>
            {categories
                // If we're showing all categories, there's no end to the slice.
                // Otherwise, just show the default number.
                .slice(0, showingAllCategories
                        ? undefined
                        : DEFAULT_NUM_CATEGORIES)
                .map(c => <CategoryCard key={c} categoryId={c} text={c} />)}
            {!showingAllCategories
                && <LoadMoreButton onClick={() => setShowingAllCategories(true)}>
                Load more
                </LoadMoreButton>}
        </DiscoverSection>
        <DiscoverSection name='Suggested' sectionId='suggested' subtitle={SuggestedSubtitle} />
        <DiscoverSection name='Newly added' sectionId='new' />
    </Flex>
}
