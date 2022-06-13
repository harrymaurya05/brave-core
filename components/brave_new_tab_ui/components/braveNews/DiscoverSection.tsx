import * as React from 'react';
import styled from 'styled-components';
import Flex from '../Flex';

interface Props {
    name: string;
    subtitle?: React.ReactNode;
    sectionId: string;
    children?: React.ReactNode;
}

const Container = styled(Flex)`
    padding: 16px 0;
`

const Header = styled.span`
    font-weight: 600;
    font-size: 16px;
`

const Link = styled.a`
    color: #4C54D2;
    font-size: 12px;
    font-weight: 600;
`

const Subtitle = styled.span`
    font-size: 12px;
`

const ItemsContainer = styled.div`
    display: grid;
    grid-template-columns: repeat(3, minmax(0, 208px));
    grid-template-rows: repeat(auto-fit, 102px);
    gap: 16px;
`

export default function DiscoverSection(props: Props) {
    return <Container direction='column'>
        <Flex direction='row' gap={8} align='center'>
            <Header>{props.name}</Header>
            <Link>View All</Link>
        </Flex>
        {props.subtitle && <Subtitle>
            {props.subtitle}
        </Subtitle>}
        <ItemsContainer>
            {props.children}
        </ItemsContainer>
    </Container>
}
