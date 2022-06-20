import * as React from 'react';
import styled from 'styled-components';
import Flex from '../Flex';
import { useBraveNews } from './Context';

const Container = styled(Flex) <{ backgroundUrl?: string }>`
    background-image: url("${p => p.backgroundUrl}");
    color: white;
    border-radius: 8px;

    :hover {
        opacity: 0.8;
    }
`

const Text = styled.span`
    font-weight: 600;
    font-size: 14px;
`

interface Props {
    categoryId: string;
    text: string;
    backgroundUrl?: string;
}

export default function CategoryCard(props: Props) {
    const { setPage } = useBraveNews();

    return <Container align='center' justify='center' backgroundUrl={props.backgroundUrl} onClick={() => setPage(`category/${props.categoryId}`)}>
        <Text>{props.text}</Text>
    </Container>
}
