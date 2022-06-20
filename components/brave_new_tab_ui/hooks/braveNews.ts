import { useMemo } from "react";
import getBraveNewsController, { Publisher } from "../api/brave_news";
import { useResult } from "./promise";

const controller = getBraveNewsController();

export default function usePublishers(categoryId?: string) {
    const { result: publishers } = useResult(async () => controller?.getPublishers(), []);
    return useMemo(() => (Object
        .values(publishers?.publishers ?? {}) as Publisher[])
        .filter(p => !categoryId || p.categoryName === categoryId),
        [publishers?.publishers, categoryId]);
}

export function useCategories() {
    const publishers = usePublishers();
    const categories = useMemo(() => Array.from(publishers.reduce((prev, next) => {
        prev.add(next.categoryName);
        return prev;
    }, new Set<string>())), [publishers]);
    return categories;
}

export function usePublisher(id: string) {
    const { result } = useResult(controller?.getPublishers, [controller?.getPublishers]);
    return result?.[id];
}
