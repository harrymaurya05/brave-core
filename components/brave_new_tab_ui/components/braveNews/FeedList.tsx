
import * as React from "react";
import styled from "styled-components";
import { isPublisherEnabled } from "../../api/brave_news";
import usePublishers from "../../hooks/braveNews";
import Flex from "../Flex";
import FeedListEntry from "./FeedListEntry";

const Title = styled.span`
  font-size: 18px;
  font-weight: 800;
  line-height: 36px;
`;

const Subtitle = styled.span`
  font-weight: 500;
  font-size: 12px;
  color: #868e96;
`;

export default function FeedList() {
    const publishers = usePublishers()
        .filter(isPublisherEnabled);

    return <div>
        <Flex direction="row" justify="space-between" align="center">
            <Title>Following</Title>
            <Subtitle>{publishers.length} sources</Subtitle>
        </Flex>
        <Flex direction="column">
            {publishers.map((p) => (
                <FeedListEntry key={p.publisherId} publisher={p} />
            ))}
        </Flex>
    </div>
}
