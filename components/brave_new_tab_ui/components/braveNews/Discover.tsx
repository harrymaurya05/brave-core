import * as React from 'react';
import styled from 'styled-components';
import Flex from '../Flex';
import DiscoverSection from './DiscoverSection';

const Header = styled.span`
    font-size: 24px;
    font-weight: 600;
    padding: 12px 0;
`

const SearchInput = styled.input`
    margin: 16px 0;
`

const Link = styled.a`
    color: #4C54D2;
    font-weight: 600;
`
const SuggestedSubtitle = <>
    Some sources you might like. Suggestions are anonymous, and matched only on your device.&nbsp;
    <Link>Learn more</Link>
</>

export default function Discover() {
    return <Flex direction='column'>
        <Header>Discover</Header>
        <SearchInput type="search"/>
        <DiscoverSection name='Trending' sectionId='trending'  />
        <DiscoverSection name='Browse by category' sectionId='categories'  />
        <DiscoverSection name='Suggested' sectionId='suggested' subtitle={SuggestedSubtitle} />
    </Flex>
}
