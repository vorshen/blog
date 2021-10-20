class OBB {
    constructor(centerX, centerY, width, height, rotation) {
        this.centerX = centerX;
        this.centerY = centerY;
        this.width = width;
        this.height = height;
        this.rotation = rotation;

        if (width <= 0 || height <= 0) {
            throw new Error(`invalid rectangle, width: ${width}, height: ${height}`);
        }
    }

    contains(point) {
        const { x, y } = point;

        const rx = (x - this.centerX) * Math.cos(-this.rotation) - (y - this.centerY) * Math.sin(-this.rotation);
        const ry = (x - this.centerX) * Math.sin(-this.rotation) + (y - this.centerY) * Math.cos(-this.rotation);

        return (ry + this.centerY > this.centerY - (this.height / 2))
            && (ry + this.centerY < this.centerY + (this.height / 2))
            && (rx + this.centerX > this.centerX - (this.width / 2))
            && (rx + this.centerX < this.centerX + (this.width / 2));
    }

    getPoints() {
        const halfWidth = this.width / 2;
        const halfHeight = this.height / 2;
        const result = [
            this.centerX - halfWidth,
            this.centerY - halfHeight,

            this.centerX + halfWidth,
            this.centerY - halfHeight,

            this.centerX + halfWidth,
            this.centerY + halfHeight,

            this.centerX - halfWidth,
            this.centerY + halfHeight,            
        ];

        for (let i = 0; i < result.length; i += 2) {
            const x = result[i];
            const y = result[i + 1];
            const rx = (x - this.centerX) * Math.cos(this.rotation) - (y - this.centerY) * Math.sin(this.rotation);
            const ry = (x - this.centerX) * Math.sin(this.rotation) + (y - this.centerY) * Math.cos(this.rotation);
            result[i] = rx + this.centerX;
            result[i + 1] = ry + this.centerY;
        }

        return result;
    }
};
