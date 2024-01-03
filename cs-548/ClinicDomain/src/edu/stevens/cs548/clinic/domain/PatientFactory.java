package edu.stevens.cs548.clinic.domain;

import java.util.Date;

public class PatientFactory implements IPatientFactory {

    @Override
    public Patient createPatient(long pid, String name, Date dob) {
        Patient newPatient = new Patient();
        newPatient.setPatientId(pid);
        newPatient.setName(name);
        newPatient.setBirthDate(dob);
        return newPatient;
    }

}
