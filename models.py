from datetime import datetime

from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()


class Biomedical(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    oxygen = db.Column(db.Float, nullable=False)
    heart_beat = db.Column(db.Float, nullable=False)
    save_time = db.Column(db.DateTime, nullable=False, default=datetime.now)

    def __init__(self, oxygen, heart_beat) -> None:
        self.oxygen = oxygen
        self.heart_beat = heart_beat

    def __repr__(self):
        return f'<Biomedical {self.oxygen} {self.heart_beat}>'

    def as_dict(self):
        return {c.name: getattr(self, c.name) for c in self.__table__.columns}
