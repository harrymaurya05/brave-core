import * as React from 'react';
import styled from 'styled-components';
import Flex from "../Flex";
import { Heart, HeartOutline } from './Icons';
import getBraveNewsController, { isPublisherEnabled, Publisher, setPublisherEnabled } from '../../api/brave_news';
import { useCachedValue } from '../../hooks/useCachedValue';

interface Props {
    publisher: Publisher;
}

const Container = styled(Flex)`
    padding: 10px 0;
    cursor: pointer;
    
    :hover {
        opacity: 0.5;
    }
`

const FavIcon = styled.img`
    width: 24px;
    height: 24px;
    border-radius: 100px;
`
const ToggleButton = styled.button`
    all: unset;
    cursor: pointer;
    color: var(--interactive5);
`

const Text = styled.span`
    font-size: 14px;
    font-weight: 500;
`

window['braveNews'] = getBraveNewsController();

export default function FeedListEntry(props: Props) {
    const { publisher } = props;

    const [following, setFollowing] = useCachedValue(isPublisherEnabled(props.publisher), newValue => setPublisherEnabled(props.publisher, newValue));

    return <Container direction="row" justify="space-between" align='center' onClick={() => setFollowing(!following)}>
        <Flex align='center' gap={8}>
            <FavIcon src={''} />
            <Text>{publisher.publisherName}</Text>
        </Flex>
        <ToggleButton>
            {following ? Heart : HeartOutline}
        </ToggleButton>
    </Container>
}
