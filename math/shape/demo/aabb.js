class AABB {  
    constructor(x, y, width, height) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
  
        if (width <= 0 || height <= 0) {
            throw new Error(`invalid rectangle, width: ${width}, height: ${height}`);
        }
    }
  
    get left() {
        return this.x;
    }
  
    get right() {
        return this.x + this.width;
    }
  
    get top() {
        return this.y;
    }
  
    get bottom() {
        return this.y + this.height;
    }
  
    contains(point) {
        if (this.width <= 0 || this.height <= 0) {
            return false;
        }
  
        const { x, y } = point;
        if (
            x >= this.left
            && x <= this.right
            && y >= this.top
            && y <= this.bottom
        ) {
            return true;
        }
  
        return false;
    }
};
  