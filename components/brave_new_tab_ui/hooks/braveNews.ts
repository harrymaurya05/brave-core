import { useMemo } from "react";
import getBraveNewsController, { Publisher } from "../api/brave_news";
import { useResult } from "./promise";

const controller = getBraveNewsController();

export default function usePublishers() {
    const { result: publishers } = useResult(async () => controller?.getPublishers(), []);
    return useMemo(() => Object.values(publishers?.publishers ?? {}) as Publisher[], [publishers?.publishers]);
}

export function usePublisher(id: string) {
    const { result } = useResult(controller?.getPublishers, [controller?.getPublishers]);
    return result?.[id];
}
