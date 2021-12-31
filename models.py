from datetime import datetime

from flask_sqlalchemy import SQLAlchemy


db = SQLAlchemy()


class OxygenData(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    value = db.Column(db.Integer, nullable=False)
    save_time = db.Column(db.DateTime, nullable=False, default=datetime.utcnow)

    def __init__(self, value) -> None:
        self.value = value

    def __repr__(self):
        return '<Oxygen %r%>' % self.value

    def as_dict(self):
        return {c.name: getattr(self, c.name) for c in self.__table__.columns}
