from datetime import datetime

from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()


class OxygenData(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    value = db.Column(db.Float, nullable=False)
    save_time = db.Column(db.DateTime, nullable=False, default=datetime.now)

    def __init__(self, value) -> None:
        self.value = value

    def __repr__(self):
        return f'<Oxygen {self.value}%>'

    def __str__(self):
        return f'Oxygen: {self.value}%'

    def as_dict(self):
        return {c.name: getattr(self, c.name) for c in self.__table__.columns}
