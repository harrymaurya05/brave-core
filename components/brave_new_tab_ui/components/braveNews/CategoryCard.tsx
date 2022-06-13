import * as React from 'react';
import styled from 'styled-components';
import Flex from '../Flex';

const Container = styled(Flex) <{ background: string }>`
    background-image: url("${p => p.background}");
    color: white;
    border-radius: 8px;
`

const Text = styled.span`
    font-weight: 600;
    font-size: 14px;
`

interface Props {
    categoryId: string;
    text: string;
    backgroundUrl: string;
}

export default function CategoryCard(props: Props) {
    return <Container align='center' justify='center' background={props.backgroundUrl}>
        <Text>{props.text}</Text>
    </Container>
}
