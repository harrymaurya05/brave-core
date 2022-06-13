import * as React from 'react';
import styled from 'styled-components';
import Flex from '../Flex';

interface Props {
    name: string;
    subtitle?: React.ReactNode;
    sectionId: string;
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

export default function DiscoverSection(props: Props) {
    return <Container direction='column'>
        <Flex direction='row' gap={8} align='center'>
            <Header>{props.name}</Header>
            <Link>View All</Link>
        </Flex>
        <Subtitle>
            {props.subtitle}
        </Subtitle>
    </Container>
}
