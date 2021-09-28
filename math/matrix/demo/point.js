class Point {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }

    apply(matrix) {
        const tempX = this.x;
        const tempY = this.y;

        this.x = matrix.a * tempX + matrix.c * tempY + matrix.tx;
        this.y = matrix.b * tempX + matrix.d * tempY + matrix.ty;
    }

    clone() {
        return new Point(this.x, this.y);
    }
};
