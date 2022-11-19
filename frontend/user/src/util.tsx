

export type MI<T extends { encode: (...args: any) => any}> = T['encode'] extends (...args: infer A) => any ? A[0] : never;
