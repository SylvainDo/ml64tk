type StringBase = 2 | 8 | 10 | 16;
type BitwiseWidth = 8 | 16 | 32 | 64;

type LargeIntegerOperand = LargeInteger | number | string | BigInt | [number /* lowPart */, number /* highPart */] | [string, StringBase];

interface LargeInteger {
    toNumber(): number;
    toString(): string;
    toString(base: number): string;
    toBigInt(): BigInt;

    lowPart: number; // get, set
    highPart: number; // get, set
    bytes: ArrayBuffer; // get

    neg(): LargeInteger;
    add(rhs: LargeIntegerOperand): LargeInteger;
    sub(rhs: LargeIntegerOperand): LargeInteger;
    mul(rhs: LargeIntegerOperand): LargeInteger;
    div(rhs: LargeIntegerOperand): LargeInteger;
    mod(rhs: LargeIntegerOperand): LargeInteger;
    band(rhs: LargeIntegerOperand): LargeInteger;
    bor(rhs: LargeIntegerOperand): LargeInteger;
    xor(rhs: LargeIntegerOperand): LargeInteger;
    not(): LargeInteger;
    shl(shift: number): LargeInteger;
    shr(shift: number): LargeInteger;
    eq(rhs: LargeIntegerOperand): boolean;
    neq(rhs: LargeIntegerOperand): boolean;
    lt(rhs: LargeIntegerOperand): boolean;
    lte(rhs: LargeIntegerOperand): boolean;
    gt(rhs: LargeIntegerOperand): boolean;
    gte(rhs: LargeIntegerOperand): boolean;
    bswap(): LargeInteger;
    bswap(width: BitwiseWidth): LargeInteger;
    bceil(): LargeInteger;
    bfloor(): LargeInteger;
    bwidth(): number;
    rotl(shift: number): LargeInteger;
    rotl(shift: number, width: BitwiseWidth): LargeInteger;
    rotr(shift: number): LargeInteger;
    rotr(shift: number, width: BitwiseWidth): LargeInteger;
    countlZero(): number;
    countlOne(): number;
    countrZero(): number;
    countrOne(): number;
    popCount(): number;
}

interface LargeIntegerConstructors {
    new(): LargeInteger;
    new(value: LargeIntegerOperand): LargeInteger;
}

export var SignedLargeInteger: LargeIntegerConstructors;
export var UnsignedLargeInteger: LargeIntegerConstructors;
