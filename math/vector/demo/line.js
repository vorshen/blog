class Line {
    constructor(p0, p1) {
        this.p0 = p0;
        this.p1 = p1;

        this.color = '#000000';
    }

    setColor(color) {
        this.color = color;
    }

    isCrossLine(line) {
        const a = this.p0;
        const b = this.p1;
        const c = line.p0;
        const d = line.p1;

        const ca_cb = (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
        const da_db = (a.x - d.x) * (b.y - d.y) - (a.y - d.y) * (b.x - d.x);
        if (ca_cb * da_db > 0) {
            return false;
        }

        const ac_ad = (c.x - a.x) * (d.y - a.y) - (c.y - a.y) * (d.x - a.x);
        const bc_bd = ac_ad + ca_cb - da_db;
        if (ac_ad * bc_bd > 0) {
            return false;
        }

        return true;
    }
};
