class Matrix {
  static matrixPool = [];

  static createInstance(a = 1, b = 0, c = 0, d = 1, tx = 0, ty = 0) {
    let matrix = Matrix.matrixPool.pop();
    if (!matrix) {
      matrix = new Matrix(a, b, c, d, tx, ty);
    } else {
      matrix.set(a, b, c, d, tx, ty);
    }

    return matrix;
  }

  static release(matrix) {
    Matrix.matrixPool.push(matrix);
  }

  constructor(a = 1, b = 0, c = 0, d = 1, tx = 0, ty = 0) {
    this.a = a;
    this.b = b;
    this.c = c;
    this.d = d;
    this.tx = tx;
    this.ty = ty;
  }

  set(a, b, c, d, tx, ty) {
    this.a = a;
    this.b = b;
    this.c = c;
    this.d = d;
    this.tx = tx;
    this.ty = ty;

    return this;
  }

  translate(x, y) {
    this.tx += x;
    this.ty += y;

    return this;
  }

  scale(x, y) {
    this.a *= x;
    this.d *= y;
    this.c *= x;
    this.b *= y;
    this.tx *= x;
    this.ty *= y;

    return this;
  }

  rotate(angle) {
    const cos = Math.cos(angle);
    const sin = Math.sin(angle);

    const ta = this.a;
    const tc = this.c;
    const ttx = this.tx;

    this.a = (ta * cos) - (this.b * sin);
    this.b = (ta * sin) + (this.b * cos);
    this.c = (tc * cos) - (this.d * sin);
    this.d = (tc * sin) + (this.d * cos);
    this.tx = (ttx * cos) - (this.ty * sin);
    this.ty = (ttx * sin) + (this.ty * cos);

    return this;
  }

  append(matrix) {
    const ta = this.a;
    const tb = this.b;
    const tc = this.c;
    const td = this.d;

    this.a = (matrix.a * ta) + (matrix.b * tc);
    this.b = (matrix.a * tb) + (matrix.b * td);
    this.c = (matrix.c * ta) + (matrix.d * tc);
    this.d = (matrix.c * tb) + (matrix.d * td);

    this.tx = (matrix.tx * ta) + (matrix.ty * tc) + this.tx;
    this.ty = (matrix.tx * tb) + (matrix.ty * td) + this.ty;

    return this;
  }

  prepend(matrix) {
    const ttx = this.tx;

    if (matrix.a !== 1 || matrix.b !== 0 || matrix.c !== 0 || matrix.d !== 1) {
        const ta = this.a;
        const tc = this.c;

        this.a = (ta * matrix.a) + (this.b * matrix.c);
        this.b = (ta * matrix.b) + (this.b * matrix.d);
        this.c = (tc * matrix.a) + (this.d * matrix.c);
        this.d = (tc * matrix.b) + (this.d * matrix.d);
    }

    this.tx = (ttx * matrix.a) + (this.ty * matrix.c) + matrix.tx;
    this.ty = (ttx * matrix.b) + (this.ty * matrix.d) + matrix.ty;

    return this;
  }

  identity() {
    this.a = 1;
    this.b = 0;
    this.c = 0;
    this.d = 0;
    this.tx = 0;
    this.ty = 0;

    return this;
  }

  invert() {
    const ta = this.a;
    const tb = this.b;
    const tc = this.c;
    const td = this.d;
    const ttx = this.tx;
    const n = (ta * td) - (tb * tc);

    this.a = td / n;
    this.b = -tb / n;
    this.c = -tc / n;
    this.d = ta / n;
    this.tx = ((tc * this.ty) - (td * ttx)) / n;
    this.ty = -((ta * this.ty) - (tb * ttx)) / n;

    return this;
  }

  clone() {
    return Matrix.createInstance(this.a, this.b, this.c, this.d, this.tx, this.ty);
  }

  copyFrom(matrix) {
    this.a = matrix.a;
    this.b = matrix.b;
    this.c = matrix.c;
    this.d = matrix.d;
    this.tx = matrix.tx;
    this.ty = matrix.ty;

    return this;
  }
};
