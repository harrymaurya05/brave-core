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

export default function Discover(props: {}) {
    const categories = useCategories();

    return <Flex direction='column'>
        <Header>Discover</Header>
        <SearchInput type="search" placeholder='Search for news, site, topic or RSS feed' />
        <DiscoverSection name='Trending' sectionId='trending' >
            <SourceCard icon={Rocket} text="Popular" id='popular' background='#353DAB' />
            <SourceCard icon={History} text="Newly added" id='new' background='#207DC9' />
            <SourceCard icon={DoubleHeart} text="Suggested" id='suggested' background='#FB542B' />
        </DiscoverSection>
        <DiscoverSection name='Browse by category' sectionId='categories'>
            {categories.map(c => <CategoryCard key={c} categoryId={c} text={c} />)}
            <LoadMoreButton onClick={console.log}>
                Load more
            </LoadMoreButton>
        </DiscoverSection>
        <DiscoverSection name='Suggested' sectionId='suggested' subtitle={SuggestedSubtitle} />
        <DiscoverSection name='Newly added' sectionId='new' />
    </Flex>
}
